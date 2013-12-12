// -*- c++ -*-
#ifndef SMC_WORLD_LOADER_H
#define SMC_WORLD_LOADER_H
#include "../core/global_game.h"
#include "../core/xml_attributes.h"

namespace SMC {

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

    // Parse the given world directory. Use this function instead of bare xmlpp’s
    // parse_file() that accepts a Glib::ustring — this function sets
    // some internal members.
    virtual void parse_dir(boost::filesystem::path dirname);

    cOverworld* Get_Overworld();
  protected: // SAX parser callbacks
    virtual void on_start_document();
    virtual void on_end_document();
    virtual void on_start_element(const Glib::ustring& name, const xmlpp::SaxParser::AttributeList& properties);
    virtual void on_end_element(const Glib::ustring& name);

  private:
    cOverworld* mp_world;
    // The directory we’re parsing
    boost::filesystem::path m_worlddir;
    // The <property> results we found before the current tag.
    XmlAttributes m_current_properties;
  };

}

#endif
