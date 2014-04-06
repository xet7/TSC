// -*- c++ -*-
#ifndef SMC_WORLD_DESCRIPTION_LOADER_HPP
#define SMC_WORLD_DESCRIPTION_LOADER_HPP
#include "../core/global_game.hpp"
#include "../core/xml_attributes.hpp"
#include "overworld.hpp"

namespace SMC {

  class cOverworldDescriptionLoader: public xmlpp::SaxParser
  {
  public:
    cOverworldDescriptionLoader();
    virtual ~cOverworldDescriptionLoader();

    // Parse the given filename.
    virtual void parse_file(boost::filesystem::path filename);

    cOverworld_description* Get_Overworld_Description();

  protected:
    // SAX parser callbacks
    virtual void on_start_document();
    virtual void on_end_document();
    virtual void on_start_element(const Glib::ustring& name, const xmlpp::SaxParser::AttributeList& properties);
    virtual void on_end_element(const Glib::ustring& name);

    void handle_world();

  private:
    // The description we’re building.
    cOverworld_description* mp_description;
    // The file we’re parsing.
    boost::filesystem::path m_descfile;
    // The <property> results we found before the current tag.
    XmlAttributes m_current_properties;
  };

}

#endif
