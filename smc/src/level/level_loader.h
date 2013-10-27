// -*- c++ -*-
#ifndef SMC_LEVEL_LOADER_H
#define SMC_LEVEL_LOADER_H
#include "../core/global_basic.h"
#include "../core/global_game.h"
#include "level.h"

namespace SMC {

	/**
	 * This class is used to construct a level from a given XML file.
	 * While technically all its code could be included in cLevel directly,
	 * having it as a separate class is much cleaner and doesn’t clutter cLevel
	 * with all the parsing stuff which is for the actual work as a Level
	 * represenative quite unimportant.
	 *
	 * Note a cLevelLoader can only be used to parse a given file once.
	 * As it internally allocates a cLevel object and exposes it after
	 * parsing, we cannot be sure if we can securely delete it before
	 * a second run, which is therefore simply forbidden and will result
	 * in an XML_Double_Parsing exception to be thrown.
	 */
	class cLevelLoader: public xmlpp::SaxParser
	{
	public:
		cLevelLoader(boost::filesystem::path levelfile);
		virtual ~cLevelLoader();

		// After finishing parsing, contains a pointer to a cLevel instance.
		// This pointer must be freed by you. Returns NULL before parsing.
		cLevel* Get_Level();

	protected: // SAX parser callbacks
		virtual void on_start_document();
		virtual void on_end_document();
		virtual void on_start_element(const Glib::ustring& name, const xmlpp::SaxParser::AttributeList& properties);
		virtual void on_end_element(const Glib::ustring& name);
		virtual void on_characters(const Glib::ustring& text);

	private:
		void Parse_Tag_Information();
		void Parse_Tag_Settings();
		void Parse_Tag_Background();
		void Parse_Tag_Player();
		void Parse_Level_Object_Tag(const std::string& name);
		/* cSprite* Create_Level_Object_From_XML(const std::string& name); */

		// The cLevel instance we’re building
		cLevel* mp_level;
		// The file we’re parsing
		boost::filesystem::path m_levelfile;
		// The <property> results we found before the current tag. The
		// value of the `name' attribute is mapped to the value of the
		// `value' attribute. on_end_element() must clear this at its end.
		XmlAttributes m_current_properties;
		// True if we’re currently parsing a <script> tag.
		bool m_in_script_tag;
	};

}

#endif
