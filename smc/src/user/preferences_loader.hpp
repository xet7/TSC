// -*- c++ -*-
#ifndef SMC_PREFERENCES_LOADER_H
#define SMC_PREFERENCES_LOADER_H
#include "../core/global_game.hpp"
#include "../core/xml_attributes.hpp"
#include "preferences.hpp"

namespace SMC {

	/**
	 * XML parser for the preferences file. You should not use
	 * this class directly, use cPreferences::Load_From_File() instead.
	 */
	class cPreferencesLoader: public xmlpp::SaxParser
	{
	public:
		cPreferencesLoader();
		virtual ~cPreferencesLoader();

		// Parse the file given by path.
		virtual void parse_file(boost::filesystem::path filename);

		cPreferences* Get_Preferences();
		float Get_Video_Geometry_Detail();
		float Get_Video_Texture_Detail();
		int Get_Audio_Music_Volume();
		int Get_Audio_Sound_Volume();

	protected:
		// SAX parser callbacks
		virtual void on_start_document();
		virtual void on_end_document();
		virtual void on_start_element(const Glib::ustring& name, const xmlpp::SaxParser::AttributeList& properties);
		//virtual void on_end_element(const Glib::ustring& name);

		void handle_property(const std::string& name, const std::string& value);

		// The preferences we’re building
		cPreferences* mp_preferences;
		// The file we’re parsing
		boost::filesystem::path m_preferences_file;
		// The <property> results we found before the current tag
		XmlAttributes m_current_properties;

		// Special preferences
		float m_video_geometry_detail;
		float m_video_texture_detail;
		int m_audio_music_volume;
		int m_audio_sound_volume;
	};

}

#endif
