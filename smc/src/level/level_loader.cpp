#include "level_loader.h"
#include "level_player.h"
#include "../core/sprite_manager.h"
#include "../core/property_helper.h"
#include "../core/filesystem/resource_manager.h"
#include "../video/font.h"
#include "../objects/enemystopper.h"
#include "../objects/level_exit.h"
#include "../objects/bonusbox.h"
#include "../objects/spinbox.h"
#include "../objects/text_box.h"
#include "../objects/goldpiece.h"

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
	else if (name == "background")
		Parse_Tag_Background();
	else if (name == "player")
		Parse_Tag_Player();
	else if (cLevel::Is_Level_Object_Element(std::string(name))) // CEGUI doesn’t like Glib::ustring
		Parse_Level_Object_Tag(name);
	else if (name == "level")
		{ /* Ignore the root <level> tag */ }
	else if (name == "script")
		m_in_script_tag = false; // Indicate the <script> tag has ended
	else
		std::cerr << "Warning: Unknown XML tag '" << name << "'on level parsing." << std::endl;

	// Everything handled, so we can now safely clear the
	// collected <property> element values for the next
	// tag.
	m_current_properties.clear();
}

void cLevelLoader::on_characters(const Glib::ustring& text)
{
	/* If we’re currently in the <script> tag, read its
	 * text (may be called multiple times for each token,
	 * so append rather then set directly). */
	if (m_in_script_tag)
		mp_level->m_script.append(text);
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

void cLevelLoader::Parse_Tag_Background()
{
	BackgroundType bg_type = static_cast<BackgroundType>(string_to_int(m_current_properties["type"]));

	// Use gradient background
	if (bg_type == BG_GR_HOR || bg_type == BG_GR_VER)
		mp_level->m_background_manager->Get_Pointer(0)->Load_From_Attributes(m_current_properties);
	else // default background
		mp_level->m_background_manager->Add(new cBackground(m_current_properties, mp_level->m_sprite_manager));
}

void cLevelLoader::Parse_Tag_Player()
{
	// Defaults
	mp_level->m_player_start_pos_x = cLevel_Player::m_default_pos_x;
	mp_level->m_player_start_pos_y = cLevel_Player::m_default_pos_y;

	if (m_current_properties.count("posx"))
		mp_level->m_player_start_pos_x = string_to_float(m_current_properties["posx"]);
	if (m_current_properties.count("posy"))
		mp_level->m_player_start_pos_y = string_to_float(m_current_properties["posy"]);

	// If V1.9 and lower: Move Y coordinate bottom to 0
	if (mp_level->m_engine_version < 35)
		mp_level->m_player_start_pos_y -= 600.0f;

	// Adjust with Y offsets other versions had
	if (mp_level->m_engine_version <= 10)
		mp_level->m_player_start_pos_y += 58.0f;
	else if (mp_level->m_engine_version <= 20)
		mp_level->m_player_start_pos_y -= 48.0f;

	// direction, set to default if invalid
	mp_level->m_player_start_direction = Get_Direction_Id(m_current_properties["direction"]);
	if (mp_level->m_player_start_direction != DIR_LEFT && mp_level->m_player_start_direction != DIR_RIGHT)
		mp_level->m_player_start_direction = DIR_RIGHT;
}

void cLevelLoader::Parse_Level_Object_Tag(const std::string& name)
{
	// create sprite
	std::vector<cSprite*> sprites = Create_Level_Objects_From_XML_Tag(name, m_current_properties, mp_level->m_engine_version, mp_level->m_sprite_manager);

	// valid
	if (sprites.size() > 0) {
		/* If a static UID has been set, assign it to the sprite. -1
		 * (cSprite’s default) means "no UID set" and instructs
		 * cSprite_Manager to assign a new, free one to the
		 * object. Note that in the unlikely case of intermixed static
		 * and dynamic UIDs (i.e.  some sprites have UIDs assigned,
		 * others not — this can only be achieved by editing the level
		 * XML by hand, the editor does not allow this) it could
		 * happen that cSprite_Manager’s UID generator generates a UID
		 * (and assignes it to the object) that is later set as a
		 * static UID for another sprite — causing this sprite to have
		 * the same UID as the one with the generated UID (this will
		 * print a warning to the console if SMC is compiled in debug
		 * mode). We cannot know this in advance, but as said you have
		 * to edit the XML by hand and therefore we can ignore this
		 * case safely. */
		if (m_current_properties.count("uid"))
			sprites[0]->m_uid = string_to_int(m_current_properties["uid"]); // The 98% case is that we get only one sprite back, the other 2% are backward compatibility

		for(std::vector<cSprite*>::iterator iter = sprites.begin(); iter != sprites.end(); iter++)
			mp_level->m_sprite_manager->Add(*iter);
	}
}

/***************************************
 * Create_Level_Objects_From_XML_Tag()
 ***************************************/

std::vector<cSprite*> cLevelLoader::Create_Level_Objects_From_XML_Tag(const std::string& name, XmlAttributes& attributes, int engine_version, cSprite_Manager* p_sprite_manager)
{
	if (name == "sprite")
		return Create_Sprites_From_XML_Tag(name, attributes, engine_version, p_sprite_manager);
	else if (name == "enemystopper")
		return Create_Enemy_Stoppers_From_XML_Tag(name, attributes, engine_version, p_sprite_manager);
	else if (name == "levelexit")
		return Create_Level_Exits_From_XML_Tag(name, attributes, engine_version, p_sprite_manager);
	else if (name == "level_entry")
		return Create_Level_Entries_From_XML_Tag(name, attributes, engine_version, p_sprite_manager);
	else if (name == "box")
		return Create_Boxes_From_XML_Tag(name, attributes, engine_version, p_sprite_manager);
	else if (name == "item" || name == "powerup") // powerup is pre V.0.99.5
		return Create_Items_From_XML_Tag(name, attributes, engine_version, p_sprite_manager);
	// FIXME: CONTINUE HERE with stuff from cLevel.cpp (Create_Level_Object_From_XML())

	// keep above list sync with cLevel::Is_Level_Object_Element()

	// This is not a level object tag, return empty list
	return std::vector<cSprite*>();
}

std::vector<cSprite*> cLevelLoader::Create_Sprites_From_XML_Tag(const std::string& name, XmlAttributes& attributes, int engine_version, cSprite_Manager* p_sprite_manager)
{
	std::vector<cSprite*> result;

	// V1.4 and lower: change some image paths
	if (engine_version < 25) {
		attributes.relocate_image( "game/box/stone8.png", "blocks/metal/stone_2_violet.png" );
		attributes.relocate_image( "ground/jungle_1/tree_type_1.png", "ground/jungle_1/tree/1.png" );
		attributes.relocate_image( "ground/jungle_1/tree_type_1_front.png", "ground/jungle_1/tree/1_front.png" );
		attributes.relocate_image( "ground/jungle_1/tree_type_2.png", "ground/jungle_1/tree/2.png" );
		attributes.relocate_image( "ground/yoshi_1/extra_1_blue.png", "ground/jungle_2/hedge/1_blue.png" );
		attributes.relocate_image( "ground/yoshi_1/extra_1_green.png", "ground/jungle_2/hedge/1_green.png" );
		attributes.relocate_image( "ground/yoshi_1/extra_1_red.png", "ground/jungle_2/hedge/1_red.png" );
		attributes.relocate_image( "ground/yoshi_1/extra_1_yellow.png", "ground/jungle_2/hedge/1_yellow.png" );
		attributes.relocate_image( "ground/yoshi_1/rope_1_leftright.png", "ground/jungle_2/rope_1_hor.png" );
	}
	// V1.5 and lower: change pipe connection image paths
	if (engine_version < 28) {
		attributes.relocate_image( "blocks/pipe/connection_left_down.png", "blocks/pipe/connection/plastic_1/orange/right_up.png" );
		attributes.relocate_image( "blocks/pipe/connection_left_up.png", "blocks/pipe/connection/plastic_1/orange/right_down.png" );
		attributes.relocate_image( "blocks/pipe/connection_right_down.png", "blocks/pipe/connection/plastic_1/orange/left_up.png" );
		attributes.relocate_image( "blocks/pipe/connection_right_up.png", "blocks/pipe/connection/plastic_1/orange/left_down.png" );
		attributes.relocate_image( "blocks/pipe/metal_connector.png", "blocks/pipe/connection/metal_1/grey/middle.png" );
	}
	// V1.7 and lower: change yoshi_1 hill_up to jungle_1 slider image paths
	if (engine_version < 31) {
		attributes.relocate_image( "ground/yoshi_1/hill_up_1.png", "ground/jungle_1/slider/2_green_left.png" );
		attributes.relocate_image( "ground/yoshi_1/hill_up_2.png", "ground/jungle_1/slider/2_blue_left.png" );
		attributes.relocate_image( "ground/yoshi_1/hill_up_3.png", "ground/jungle_1/slider/2_brown_left.png" );
	}
	// V1.7.x and lower: change green_1 ground to green_3 ground image paths
	if (engine_version < 34) {
		// normal
		attributes.relocate_image( "ground/green_1/ground/left_up.png", "ground/green_3/ground/top/left.png" );
		attributes.relocate_image( "ground/green_1/ground/left_down.png", "ground/green_3/ground/bottom/left.png" );
		attributes.relocate_image( "ground/green_1/ground/right_up.png", "ground/green_3/ground/top/right.png" );
		attributes.relocate_image( "ground/green_1/ground/right_down.png", "ground/green_3/ground/bottom/right.png" );
		attributes.relocate_image( "ground/green_1/ground/up.png", "ground/green_3/ground/top/1.png" );
		attributes.relocate_image( "ground/green_1/ground/down.png", "ground/green_3/ground/bottom/1.png" );
		attributes.relocate_image( "ground/green_1/ground/right.png", "ground/green_3/ground/middle/right.png" );
		attributes.relocate_image( "ground/green_1/ground/left.png", "ground/green_3/ground/middle/left.png" );
		attributes.relocate_image( "ground/green_1/ground/middle.png", "ground/green_3/ground/middle/1.png" );

		// hill (not available)
		//attributes.relocate_image( "ground/green_1/ground/hill_left_up.png", "ground/green_3/ground/" );
		//attributes.relocate_image( "ground/green_1/ground/hill_right_up.png", "ground/green_3/ground/" );
		//attributes.relocate_image( "ground/green_1/ground/hill_right.png", "ground/green_3/ground/" );
		//attributes.relocate_image( "ground/green_1/ground/hill_left.png", "ground/green_3/ground/" );
	}
	// V1.9 and lower: move y coordinate bottom to 0
	if (engine_version < 35) {
		if (attributes.count("posy") > 0)
			attributes["posy"] = float_to_string(string_to_float(attributes["posy"]) - 600.0f);
	}
	// V1.9.x and lower: change fire-1 animation to particles
	if (engine_version < 37) {
		attributes.relocate_image( "animation/fire_1/1.png", "animation/particles/fire_1.png" );
		attributes.relocate_image( "animation/fire_1/2.png", "animation/particles/fire_2.png" );
		attributes.relocate_image( "animation/fire_1/3.png", "animation/particles/fire_3.png" );
		attributes.relocate_image( "animation/fire_1/4.png", "animation/particles/fire_4.png" );
	}
	// always: fix sprite with undefined massive-type
	if (attributes.count("type") > 0 && attributes["type"] == "undefined")
		attributes["type"] = "passive"; // So it doesn’t hinder gameplay

	cSprite* p_sprite = new cSprite(attributes, p_sprite_manager);

	// If image not available display its filename
	if (!p_sprite->m_start_image) {
		std::string text = attributes["image"];
		if (text.empty())
			text = "Invalid image here";

		cGL_Surface* p_text_image = pFont->Render_Text(pFont->m_font_small, text);
		p_text_image->m_path = utf8_to_path(text);
		p_sprite->Set_Image(p_text_image, true, true);
		p_sprite->Set_Sprite_Type(TYPE_FRONT_PASSIVE); // It shouldn't hinder gameplay
		p_sprite->Set_Active(false); // Only display it in the editor
	}

	// needs image
	if (p_sprite->m_image) {
		// If V1.2 and lower: change pipe position
		if (engine_version < 22) {
			if( p_sprite->m_image->m_path.compare( pResource_Manager->Get_Game_Pixmap("pipes/green/up.png")) == 0 ||
				p_sprite->m_image->m_path.compare( pResource_Manager->Get_Game_Pixmap("pipes/green/ver.png")) == 0 ||
				p_sprite->m_image->m_path.compare( pResource_Manager->Get_Game_Pixmap("pipes/green/down.png")) == 0 ||
				p_sprite->m_image->m_path.compare( pResource_Manager->Get_Game_Pixmap("pipes/blue/up.png")) == 0 ||
				p_sprite->m_image->m_path.compare( pResource_Manager->Get_Game_Pixmap("pipes/blue/ver.png")) == 0 ||
				p_sprite->m_image->m_path.compare( pResource_Manager->Get_Game_Pixmap("pipes/blue/down.png")) == 0 ||
				p_sprite->m_image->m_path.compare( pResource_Manager->Get_Game_Pixmap("pipes/yellow/up.png")) == 0 ||
				p_sprite->m_image->m_path.compare( pResource_Manager->Get_Game_Pixmap("pipes/yellow/ver.png")) == 0 ||
				p_sprite->m_image->m_path.compare( pResource_Manager->Get_Game_Pixmap("pipes/yellow/down.png")) == 0 ||
				p_sprite->m_image->m_path.compare( pResource_Manager->Get_Game_Pixmap("pipes/grey/up.png")) == 0 ||
				p_sprite->m_image->m_path.compare( pResource_Manager->Get_Game_Pixmap("pipes/grey/ver.png")) == 0 ||
				p_sprite->m_image->m_path.compare( pResource_Manager->Get_Game_Pixmap("pipes/grey/down.png")) == 0 ) {
				p_sprite->Move( -6, 0, 1 );
				p_sprite->m_start_pos_x = p_sprite->m_pos_x;
			}
			else if( p_sprite->m_image->m_path.compare( pResource_Manager->Get_Game_Pixmap("pipes/green/right.png")) == 0 ||
					 p_sprite->m_image->m_path.compare( pResource_Manager->Get_Game_Pixmap("pipes/green/hor.png")) == 0 ||
					 p_sprite->m_image->m_path.compare( pResource_Manager->Get_Game_Pixmap("pipes/green/left.png")) == 0 ||
					 p_sprite->m_image->m_path.compare( pResource_Manager->Get_Game_Pixmap("pipes/blue/right.png")) == 0 ||
					 p_sprite->m_image->m_path.compare( pResource_Manager->Get_Game_Pixmap("pipes/blue/hor.png")) == 0 ||
					 p_sprite->m_image->m_path.compare( pResource_Manager->Get_Game_Pixmap("pipes/blue/left.png")) == 0 ||
					 p_sprite->m_image->m_path.compare( pResource_Manager->Get_Game_Pixmap("pipes/yellow/right.png")) == 0 ||
					 p_sprite->m_image->m_path.compare( pResource_Manager->Get_Game_Pixmap("pipes/yellow/hor.png")) == 0 ||
					 p_sprite->m_image->m_path.compare( pResource_Manager->Get_Game_Pixmap("pipes/yellow/left.png")) == 0 ||
					 p_sprite->m_image->m_path.compare( pResource_Manager->Get_Game_Pixmap("pipes/grey/right.png")) == 0 ||
					 p_sprite->m_image->m_path.compare( pResource_Manager->Get_Game_Pixmap("pipes/grey/hor.png")) == 0 ||
					 p_sprite->m_image->m_path.compare( pResource_Manager->Get_Game_Pixmap("pipes/grey/left.png")) == 0) {
				p_sprite->Move( 0, -6, 1 );
				p_sprite->m_start_pos_y = p_sprite->m_pos_y;
			}
		} // engine_version < 22

		// If V1.2.x and lower: change some hill positions
		if (engine_version < 23) {
			if( p_sprite->m_image->m_path.compare( pResource_Manager->Get_Game_Pixmap("hills/green_1/head.png")) ||
				p_sprite->m_image->m_path.compare( pResource_Manager->Get_Game_Pixmap("hills/light_blue_1/head.png")) == 0 ) {
				p_sprite->Move( 0, -6, 1 );
				p_sprite->m_start_pos_y = p_sprite->m_pos_y;
			}
		} // engine_version < 23

		// If V1.7 and lower: change yoshi_1 hill_up to jungle_1 slider image paths
		if (engine_version < 31) {
			// This is one of the XML tags that explodes into multiple sprites.
			// image filename is already changed but we need to add the middle and right tiles
			if( p_sprite_manager && ( p_sprite->m_image->m_path.compare( pResource_Manager->Get_Game_Pixmap("ground/jungle_1/slider/2_green_left.png")) == 0 ||
									  p_sprite->m_image->m_path.compare( pResource_Manager->Get_Game_Pixmap("ground/jungle_1/slider/2_blue_left.png")) == 0 ||
									  p_sprite->m_image->m_path.compare( pResource_Manager->Get_Game_Pixmap("ground/jungle_1/slider/2_brown_left.png")) == 0 )) {
				std::string color;
				// green
				if( p_sprite->m_image->m_path.compare( pResource_Manager->Get_Game_Pixmap("ground/jungle_1/slider/2_green_left.png" )) == 0 )
					color = "green";
				// blue
				else if( p_sprite->m_image->m_path.compare( pResource_Manager->Get_Game_Pixmap("ground/jungle_1/slider/2_blue_left.png") ) == 0 )
					color = "blue";
				// brown
				else
					color = "brown";

				cSprite *p_copy = p_sprite;

				// add middle tiles
				for( unsigned int i = 0; i < 4; i++ ) {
					p_copy = p_copy->Copy();
					p_copy->Set_Image( pVideo->Get_Surface( utf8_to_path( "ground/jungle_1/slider/2_" + color + "_middle.png") ), 1 );
					p_copy->Set_Pos_X( p_copy->m_start_pos_x + 22, 1 );
					//p_sprite_manager->Add( p_copy );
					result.push_back(p_copy);
				}

				// add end tile
				p_copy = p_copy->Copy();
				p_copy->Set_Image( pVideo->Get_Surface( utf8_to_path( "ground/jungle_1/slider/2_" + color + "_right.png" ) ), 1 );
				p_copy->Set_Pos_X( p_copy->m_start_pos_x + 22, 1 );
				//p_sprite_manager->Add( p_copy );
				result.push_back(p_copy);
			}
		} // engine_version < 31

		// If V1.7 and lower: change slider grey_1 to green_1 brown slider image paths
		if (engine_version < 32) {
			// image filename is already changed but we need to add an additional middle tile for left and right
			if( p_sprite_manager && ( p_sprite->m_image->m_path.compare( pResource_Manager->Get_Game_Pixmap("ground/green_1/slider/1/brown/left.png")) == 0 ||
									  p_sprite->m_image->m_path.compare( pResource_Manager->Get_Game_Pixmap("ground/green_1/slider/1/brown/right.png") ) == 0 ) ) {
				// add middle tile
				cSprite *p_copy = p_sprite->Copy();
				p_copy->Set_Image( pVideo->Get_Surface( utf8_to_path( "ground/green_1/slider/1/brown/middle.png") ), 1 );

				// if from left tile it must be moved
				if( p_sprite->m_image->m_path.compare( pResource_Manager->Get_Game_Pixmap("ground/green_1/slider/1/brown/left.png") ) == 0 )
					p_copy->Set_Pos_X( p_copy->m_start_pos_x + 18, 1 );

				//p_sprite_manager->Add( p_copy );
				result.push_back(p_copy);
			}
			// move right tile
			if( p_sprite->m_image->m_path.compare( pResource_Manager->Get_Game_Pixmap("ground/green_1/slider/1/brown/right.png") ) == 0 ) {
				p_sprite->Move( 18, 0, 1 );
				p_sprite->m_start_pos_x = p_sprite->m_pos_x;
			}
		} // engine_version < 32
	} // p_sprite->m_image

	result.push_back(p_sprite);
	return result;
}

std::vector<cSprite*> cLevelLoader::Create_Enemy_Stoppers_From_XML_Tag(const std::string& name, XmlAttributes& attributes, int engine_version, cSprite_Manager* p_sprite_manager)
{
	std::vector<cSprite*> result;

	// If V1.9 and lower: Move Y coordinate bottom to 0
	if (engine_version < 35 && attributes.count("posy") > 0)
		attributes["posy"] = float_to_string(string_to_float(attributes["posy"]) - 600.0f);

	result.push_back(new cEnemyStopper(attributes, p_sprite_manager));
	return result;
}

std::vector<cSprite*> cLevelLoader::Create_Level_Exits_From_XML_Tag(const std::string& name, XmlAttributes& attributes, int engine_version, cSprite_Manager* p_sprite_manager)
{
	std::vector<cSprite*> result;

	// If V1.9 and lower: Move Y coordinate bottom to 0
	if (engine_version < 35 && attributes.count("posy") > 0)
		attributes["posy"] = float_to_string(string_to_float(attributes["posy"]) - 600.0f);

	// If V1.9.x and lower: change "motion" to "camera_motion"
	if (engine_version < 36 && attributes.count("motion") > 0) {
		attributes["camera_motion"] = int_to_string(string_to_int(attributes["motion"]) + 1);
		attributes.erase("motion");
	}

	result.push_back(new cLevel_Exit(attributes, p_sprite_manager));
	return result;
}

std::vector<cSprite*> cLevelLoader::Create_Level_Entries_From_XML_Tag(const std::string& name, XmlAttributes& attributes, int engine_version, cSprite_Manager* p_sprite_manager)
{
	std::vector<cSprite*> result;

	// If V1.9 and lower: Move Y coordinate bottom to 0
	if (engine_version < 35 && attributes.count("posy") > 0)
		attributes["posy"] = float_to_string(string_to_float(attributes["posy"]) - 600.0f);

	result.push_back(new cLevel_Entry(attributes, p_sprite_manager));
	return result;
}

std::vector<cSprite*> cLevelLoader::Create_Boxes_From_XML_Tag(const std::string& name, XmlAttributes& attributes, int engine_version, cSprite_Manager* p_sprite_manager)
{
	std::vector<cSprite*> result;

	// If V1.9 and lower: Move Y coordinate bottom to 0
	if (engine_version < 35 && attributes.exists("posy"))
		attributes["posy"] = float_to_string(string_to_float(attributes["posy"]) - 600.0f);

	if (attributes["type"] == "bonus")
		result.push_back(new cBonusBox(attributes, p_sprite_manager));
	else if (attributes["type"] == "gold"){ // `gold' is somewhere pre V0.99.5
		// Update old values
		attributes["type"]      = "bonus";
		attributes["animation"] = "Default";
		attributes["item"]      = int_to_string(TYPE_GOLDPIECE);

		// Renamed old values
		if (attributes.exists("color")) {
			attributes["gold_color"] = attributes["color"];
			attributes.erase("color");
		}

		result.push_back(new cBonusBox(attributes, p_sprite_manager));
	}
	else if (attributes["type"] == "spin")
		result.push_back(new cSpinBox(attributes, p_sprite_manager));
	else if (attributes["type"] == "text")
		result.push_back(new cText_Box(attributes, p_sprite_manager));
	else if (attributes["type"] == "empty"){ // pre V0.99.4
		// Update old values
		attributes["type"] = "bonus";
		attributes["item"] = "0";

		result.push_back(new cBonusBox(attributes, p_sprite_manager));
	}
	else if (attributes["type"] == "invisible"){ // pre V0.99.4
		// Update old values
		attributes["type"] = "bonus";
		attributes["item"] = "0";
		attributes["invisible"] = "1";

		result.push_back(new cBonusBox(attributes, p_sprite_manager));
	}
	else // if attributes["type"] == X
		std::cerr << "Warning: Unknown level box type: " << attributes["type"] << std::endl;

	return result;
}

std::vector<cSprite*> cLevelLoader::Create_Items_From_XML_Tag(const std::string& name, XmlAttributes& attributes, int engine_version, cSprite_Manager* p_sprite_manager)
{
	std::vector<cSprite*> result;

	// if V.1.9 and lower : move y coordinate bottom to 0
	if (engine_version < 35 && attributes.exists("posy"))
		attributes["posy"] = float_to_string(string_to_float(attributes["posy"]) - 600.0f);

	std::string type = attributes["type"];
	if (type == "goldpiece")
		result.push_back(new cGoldpiece(attributes, p_sprite_manager));
	else if (type == "mushroom"){/* TODO */}
	else if (type == "fireplant"){/* TODO */}
	else if (type == "jstar"){/* TODO */}
	else if (type == "moon"){/* TODO */}
	else // type == "X"
		std::cerr << "Warning: Unknown level item type '" << type << "'" << std::endl;

	return result;
}
