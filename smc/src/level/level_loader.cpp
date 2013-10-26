#include "level_loader.h"

namespace fs = boost::filesystem;
using namespace SMC;

cLevelLoader::cLevelLoader(fs::path levelfile)
	: xmlpp::SaxParser()
{
	m_levelfile = levelfile; // Copy
	mp_level    = NULL;
}

cLevelLoader::~cLevelLoader()
{
	// Do not delete the cLevel instance — it is used by the
	// caller and deleted by him.
	mp_level = NULL;
}

cLevel* cLevelLoader::Get_Level()
{
	return mp_level;
}

/***************************************
 * SAX parser callbacks
 ***************************************/

void cLevelLoader::on_start_document()
{
	if (mp_level)
		throw("Restarted XML parser after already starting it."); // FIXME: proper exception

	mp_level = new cLevel();
}

void cLevelLoader::on_end_document()
{
	// Nothing
}

void cLevelLoader::on_start_element(const Glib::ustring& name, const xmlpp::SaxParser::AttributeList& properties)
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
	else if (name == "script") {
		// Indicate a script tag has opened, so we can retrieve
		// its and only its text.
		m_in_script_tag = true;
	}
}

void cLevelLoader::on_end_element(const Glib::ustring& name)
{
	// <property> tags are parsed cumulatively in on_start_element()
	// so all have been collected when the surrounding element
	// terminates here.
	if (name == "property")
		return;

	// Everything handled, so we can now safely clear the
	// collected <property> element values for the next
	// tag.
	m_current_properties.clear();
}

void cLevelLoader::on_characters(const Glib::ustring& text)
{

}

/***************************************
 * Parsers for mayor XML tags
 ***************************************/

