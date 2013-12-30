#include "../core/i18n.h"
#include "savegame_loader.h"
#include "savegame.h"

namespace fs = boost::filesystem;
using namespace SMC;

cSavegameLoader::cSavegameLoader()
	: xmlpp::SaxParser()
{
	mp_save = NULL;
}

cSavegameLoader::~cSavegameLoader()
{
	// Do not delete the cSave instance — it is
	// used by the caller and deleted by him.
	mp_save = NULL;
}

cSave* cSavegameLoader::Get_Save()
{
	return mp_save;
}

/***************************************
 * SAX parser callbacks
 ***************************************/

void cSavegameLoader::parse_file(fs::path filename)
{
	m_savefile = filename;

	m_is_old_format = m_savefile.extension() == utf8_to_path(".smcsav") ? false : true;

	xmlpp::SaxParser::parse_file(path_to_utf8(filename));
}

void cSavegameLoader::on_start_document()
{
	if (mp_save)
		throw("Restarted XML parser after already starting it."); // FIXME: Proper exception

	mp_save = new cSave();
}

void cSavegameLoader::on_end_document()
{
	// if no description is set
	if (mp_save->m_description.empty())
		mp_save->m_description = _("No description");
}

void cSavegameLoader::on_start_element(const Glib::ustring& name, const xmlpp::SaxParser::AttributeList& properties)
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

void cSavegameLoader::on_end_element(const Glib::ustring& name)
{
	// Already handled
	if (name == "property" || name == "Property")
		return;

	if (name == "information" || name == "Information")
		handle_information();
	else
		std::cerr << "Warning: Unknown savegame element '" << name << "'" << std::endl;

	m_current_properties.clear();
}

void cSavegameLoader::handle_information()
{
	mp_save->m_version				= m_current_properties.retrieve<int>("version");
	mp_save->m_level_engine_version	= m_current_properties.fetch<int>("level_engine_version", mp_save->m_level_engine_version);
	mp_save->m_save_time			= string_to_int64(m_current_properties["save_time"]);
	mp_save->m_description			= m_current_properties["description"];
}
