// -*- c++ -*-
#ifndef SMC_WORLD_LOADER_H
#define SMC_WORLD_LOADER_H
#include "../core/global_game.h"
#include "../core/xml_attributes.h"

namespace SMC {

	/**
	 * This class holds the result of parsing a world
	 * XML file. It is used exclusively by cOverworld
	 * constructors.
	 */
	class cOverworldData {
	public:
		int m_engine_version;
		time_t m_last_saved;
	};

 /**
  * Parser for a world directory; actually employs
  * several subparsers for the XML files in that
  * directory, but handles the main XML file
  * itself.
  */
  class cOverworldLoader: public xmlpp::SaxParser
  {
  public:
    cOverworldLoader();
    virtual ~cOverworldLoader();

    // Parse the given world file. Use this function instead of bare xmlpp’s
    // parse_file() that accepts a Glib::ustring — this function sets
    // some internal members.
    virtual void parse_file(boost::filesystem::path filename);

    cOverworldData* Get_Overworld_Data();
  protected: // SAX parser callbacks
    virtual void on_start_document();
    virtual void on_end_document();
    virtual void on_start_element(const Glib::ustring& name, const xmlpp::SaxParser::AttributeList& properties);
    virtual void on_end_element(const Glib::ustring& name);

  private:
    cOverworldData* mp_data;
    // The directory we’re parsing
    boost::filesystem::path m_worldfile;
    // The <property> results we found before the current tag.
    XmlAttributes m_current_properties;
  };

}

#endif
