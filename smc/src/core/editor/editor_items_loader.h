// -*- c++ -*-
#ifndef SMC_EDITOR_ITEMS_LOADER_H
#define SMC_EDITOR_ITEMS_LOADER_H
#include "../global_game.h"
#include "../xml_attributes.h"

namespace SMC {

	class cEditorItemsLoader: public xmlpp::SaxParser
	{
	public:
		cEditorItemsLoader();
		virtual ~cEditorItemsLoader();

		// Parse the given filename. Use this function instead of bare xmlpp’s
		// parse_file() that accepts a Glib::ustring — this function sets
		// some internal members.
		virtual void parse_file(boost::filesystem::path filename);
		// Retrieve the tagged cSprite instances found by the parser.
		vector<cSprite*> get_tagged_sprites();

	protected: // SAX parser callbacks
		virtual void on_start_document();
		virtual void on_end_document();
		virtual void on_start_element(const Glib::ustring& name, const xmlpp::SaxParser::AttributeList& properties);
		virtual void on_end_element(const Glib::ustring& name);

		// This method must be overridden by subclasses. It should
		// return a sprite fitting for the given attributes.
		// That is, either use cLevelLoader::Create_Level_Objects_From_XML_Tag()
		// or the corresponding equivalent.
		virtual cSprite* get_object(const Glib::ustring& name, XmlAttributes& attributes, int engine_version);

	private:
		// The file we’re parsing
		boost::filesystem::path m_items_file;
		// The <property> results we found before the current tag. The
		// value of the `name' attribute is mapped to the value of the
		// `value' attribute. on_end_element() must clear this at its end.
		XmlAttributes m_current_properties;

		vector<cSprite*> m_tagged_sprites;
	};

}

#endif
