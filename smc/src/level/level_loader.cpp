#include "level_loader.h"
#include "../core/property_helper.h"

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

	// Now for the real, cumbersome parsing process
	if (name == "information")
		Parse_Tag_Information();
	else if (name == "settings")
		Parse_Tag_Settings();
	else {
		if (cLevel::Is_Level_Object_Element(std::string(name)))
			Parse_Level_Object_Tag(name);
		else
			std::cerr << "Warning: Unknown XML tag '" << name << "'on level parsing." << std::endl;
	}

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

void cLevelLoader::Parse_Tag_Information()
{
	// Support V1.7 and lower which used float
	float engine_version_float = string_to_float(m_current_properties["engine_version"]);

	// if float engine version
	if (engine_version_float < 3)
		engine_version_float *= 10; // change to new format

	mp_level->m_engine_version = static_cast<int>(engine_version_float);
	mp_level->m_last_saved     = string_to_int64(m_current_properties["save_time"]);
}

void cLevelLoader::Parse_Tag_Settings()
{
	// If V1.9 and lower: Move Y coordinate bottom to 0
	if (mp_level->m_engine_version < 35 && m_current_properties.count("cam_limit_h") > 0) {
		float y = string_to_float(m_current_properties["cam_limit_h"]);
		m_current_properties["cam_lmit_h"] = float_to_string(y - 600.0f);
	}

	mp_level->Set_Author(                              m_current_properties["lvl_author"]);
	mp_level->Set_Version(                             m_current_properties["lvl_version"]);
	mp_level->Set_Difficulty(  string_to_int(          m_current_properties["lvl_difficulty"]));
	mp_level->Set_Description( xml_string_to_string(   m_current_properties["lvl_description"]));
	mp_level->Set_Music(       utf8_to_path(           m_current_properties["lvl_music"]));
	mp_level->Set_Land_Type(   Get_Level_Land_Type_Id( m_current_properties["lvl_land_type"]));

	mp_level->m_fixed_camera_hor_vel = string_to_float(m_current_properties["cam_fixed_hor_vel"]);

	mp_level->m_camera_limits = GL_rect(	string_to_float(m_current_properties["cam_limit_x"]),
											string_to_float(m_current_properties["cam_limit_y"]),
											string_to_float(m_current_properties["cam_limit_w"]),
											string_to_float(m_current_properties["cam_limit_h"]));
}

void cLevelLoader::Parse_Level_Object_Tag(const std::string& name)
{
	// TODO
}
