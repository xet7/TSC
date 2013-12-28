#include "overworld_loader.h"
#include "overworld_description_loader.h"
#include "overworld.h"

namespace fs = boost::filesystem;
using namespace SMC;

cOverworldLoader::cOverworldLoader()
	: xmlpp::SaxParser()
{
	mp_overworld = NULL;
}

cOverworldLoader::~cOverworldLoader()
{
	// mp_overworld must be deleted by caller
	mp_overworld = NULL;
}

cOverworld* cOverworldLoader::Get_Overworld()
{
	return mp_overworld;
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
	if (mp_overworld)
		throw("Restarted XML parser after already starting it."); // FIXME: proper exception

	mp_overworld = new cOverworld();
}

void cOverworldLoader::on_end_document()
{
	// engine version entry not set
	if (mp_overworld->m_engine_version < 0)
		mp_overworld->m_engine_version = 0;
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

			if (attr.name == "name" || attr.name == "Name")
				key = attr.value;
			else if (attr.name == "value" || attr.name == "Value")
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

	if (name == "information")
		Parse_Tag_Information();
	else {
		// TODO
		std::cerr << "Warning: Unknown overworld element '" << name << "'" << std::endl;
	}

	// Everything handled, so we can now safely clear the
	// collected <property> element values for the next
	// tag.
	m_current_properties.clear();
}

/***************************************
 * Main tag parsers
 ***************************************/

void cOverworldLoader::Parse_Tag_Information()
{
	mp_overworld->m_engine_version = string_to_int(m_current_properties["engine_version"]);
	mp_overworld->m_last_saved = string_to_int64(m_current_properties["save_time"]);
}
