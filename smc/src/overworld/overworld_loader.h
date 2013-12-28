// -*- c++ -*-
#ifndef SMC_WORLD_LOADER_H
#define SMC_WORLD_LOADER_H
#include "../core/global_game.h"
#include "../core/xml_attributes.h"

namespace SMC {

 /**
  * Parser for a world file. This class is used
  * in conjunction with the loaders for layers
  * and description XML files for worlds by
  * the cOverworld constructor.
  *
  * Note this means that the cOverworld instance you
  * get from this parser is **incomplete**! Without
  * employing the two other XML parsers you can’t use
  * the cOverworld for playing; therefore, use cOverworld’s
  * constructor for instanciating cOverworld rather than this
  * class directly, which should be considered as a private API.
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

    cOverworld* Get_Overworld();
  protected: // SAX parser callbacks
    virtual void on_start_document();
    virtual void on_end_document();
    virtual void on_start_element(const Glib::ustring& name, const xmlpp::SaxParser::AttributeList& properties);
    virtual void on_end_element(const Glib::ustring& name);

    // Main tag parsers
    void Parse_Tag_Information();
    void Parse_Tag_Settings();

  private:
    // The cOverworld instance this parser builds up.
    cOverworld* mp_overworld;
    // The world file we’re parsing
    boost::filesystem::path m_worldfile;
    // The <property> results we found before the current tag.
    XmlAttributes m_current_properties;
  };

}

#endif
