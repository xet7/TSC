#include "campaign_loader.h"

namespace fs = boost::filesystem;
using namespace SMC;

cCampaignLoader::cCampaignLoader()
	: xmlpp::SaxParser()
{
	mp_campaign = NULL;
}

cCampaignLoader::~cCampaignLoader()
{
	// Do not delete the cCampaign instance — it is used by the
	// caller and deleted by him.
	mp_campaign = NULL;
}

cLevel* cCampaignLoader::Get_Campaign()
{
	return mp_campaign;
}

/***************************************
 * SAX parser callbacks
 ***************************************/

void cCampaignLoader::parse_file(boost::filesystem::path filename)
{
	m_campaignfile = filename;
	xmlpp::SaxParser::parse_file(path_to_utf8(filename));
}

void cCampaignLoader::on_start_document()
{
	if (mp_level)
		throw(RestartedXmlParserError());

	mp_campaign = new cCampaign();
}

void cCampaignLoader::on_end_document()
{
	//
}

void cCampaignLoader::on_start_element(const Glib::ustring& name, const xmlpp::SaxParser::AttributeList& properties)
{
	if (name == "property") {
		std::string key;
		std::string value;

		/* Collect all the <property> elements for the surrounding
		 * mayor element (like <settings> or <sprite>). When the
		 * surrounding element is closed, the results are handled
		 * in on_end_element(). */
		for(xmlpp::SaxParser::AttributeList::const_iterator iter = properties.begin(); iter != properties.end(); iter++) {
			xmlpp::SaxParser::Attribute attr = *iter;

			if (attr.name == "name")
				key = attr.value;
			else if (attr.name == "value")
				value = attr.value;
		}

		m_current_properties[key] = value;
	}
}

void cLevelLoader::on_end_element(const Glib::ustring& name)
{
	// <property> tags are parsed cumulatively in on_start_element()
	// so all have been collected when the surrounding element
	// terminates here.
	if (name == "property")
		return;

	m_current_properties.clear();
}
