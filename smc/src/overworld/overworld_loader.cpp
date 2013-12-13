#include "overworld_loader.h"
#include "overworld_description_loader.h"
#include "overworld.h"

namespace fs = boost::filesystem;
using namespace SMC;

cOverworldLoader::cOverworldLoader()
	: xmlpp::SaxParser()
{
	mp_data = NULL;
}

cOverworldLoader::~cOverworldLoader()
{
	delete mp_data;
}

cOverworldData* cOverworldLoader::Get_Overworld_Data()
{
	return mp_data;
}

/***************************************
 * SAX parser callbacks
 ***************************************/

void cOverworldLoader::parse_file(fs::path filename)
{
	m_worldfile = filename;
	xmlpp::SaxParser::parse_file(path_to_utf8(m_worldfile));
}

void cOverworldLoader::on_start_document()
{
	if (mp_data)
		throw("Restarted XML parser after already starting it."); // FIXME: proper exception

	mp_data = new cOverworldData();
}

void cOverworldLoader::on_end_document()
{
	// engine version entry not set
	if (mp_data->m_engine_version < 0)
		mp_data->m_engine_version = 0;
}

void cOverworldLoader::on_start_element(const Glib::ustring& name, const xmlpp::SaxParser::AttributeList& properties)
{
	if (name == "property" || name == "Property") {
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

void cOverworldLoader::on_end_element(const Glib::ustring& name)
{
	// <property> tags are parsed cumulatively in on_start_element()
	// so all have been collected when the surrounding element
	// terminates here.
	if (name == "property" || name == "Property")
		return;

	// Everything handled, so we can now safely clear the
	// collected <property> element values for the next
	// tag.
	m_current_properties.clear();
}
