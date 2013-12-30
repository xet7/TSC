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
	else if (name == "level" || name == "Level")
		handle_level();
	else if (name == "objects_data")
		return; // don’t clear attributes
	else if (name == "object" || name == "Level_Object") {
		handle_level_object();
		return; // don’t clear attributes
	}
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

void cSavegameLoader::handle_level()
{
	cSave_Level* p_savelevel = new cSave_Level();

	// Restore the general attributes.
	p_savelevel->m_name			= m_current_properties["level_name"];
	p_savelevel->m_mruby_data	= m_current_properties["mruby_data"];
	p_savelevel->m_level_pos_x	= m_current_properties.retrieve<float>("player_posx");
	p_savelevel->m_level_pos_y	= m_current_properties.retrieve<float>("player_poxy");

	/* Restore object lists. Note the lists in `p_savelevel' are
	 * currently empty (it’s a new object) and hence swapping
	 * with them consequently means clearing the swapping
	 * partner. */
	// set level objects
	p_savelevel->m_level_objects.swap(m_level_objects);
	// set level spawned objects
	p_savelevel->m_spawned_objects.swap(m_level_spawned_objects);

	// Add this level to the list of levels for this
	// savegame.
	mp_save->m_levels.push_back(p_savelevel);
}

void cSavegameLoader::handle_level_object()
{
	int type = m_current_properties.retrieve<int>("type");
	if (type <= 0) {
		std::cerr << "Warning: Unknown level object type '" << type << "'" << std::endl;
		return;
	}

	cSave_Level_Object* p_object = new cSave_Level_Object();

	// type
	p_object->m_type = static_cast<SpriteType>(type);

	// Get properties
	XmlAttributes::const_iterator iter;
	for (iter = m_current_properties.begin(); iter != m_current_properties.end(); iter++) {

		// Ignore level attributes
		if (iter->first == "level_name" || iter->first == "player_posx" || iter->first == "player_posy" || iter->first == "mruby_data")
			continue;

		p_object->m_properties.push_back(cSave_Level_Object_Property(iter->first, iter->second));
	}

	/* We are in a nested tag situation (<object> in <objects_data>).
	 * We now clear only the properties specific to <object> so the
	 * <property>s found in the upper <objects_data> node are preserved
	 * for the next <object>. */
	m_current_properties.erase("type");
	// remove used properties
	Save_Level_Object_ProprtyList::const_iterator iter2;
	for (iter2 = p_object->m_properties.begin(); iter2 != p_object->m_properties.end(); iter2++) {
		cSave_Level_Object_Property prop = (*iter2);
		m_current_properties.erase(prop.m_name);
	}

	// add object for handling in handle_level()
	m_level_objects.push_back(p_object);
}
