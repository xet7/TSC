// -*- c++ -*-
#ifndef SMC_SAVEGAME_LOADER_H
#define SMC_SAVEGAME_LOADER_H
#include "../core/global_game.h"
#include "../core/xml_attributes.h"
#include "savegame.h"

namespace SMC {

  class cSavegameLoader: public xmlpp::SaxParser
  {
  public:
    cSavegameLoader();
    virtual ~cSavegameLoader();

    // Parse the given filename.
    virtual void parse_file(boost::filesystem::path filename);

    cSave* Get_Save();

  protected:
    // SAX parser callbacks
    virtual void on_start_document();
    virtual void on_end_document();
    virtual void on_start_element(const Glib::ustring& name, const xmlpp::SaxParser::AttributeList& properties);
    virtual void on_end_element(const Glib::ustring& name);

    void handle_information();
    void handle_level();
    void handle_level_object();
    void handle_level_spawned_object(const Glib::ustring& name);
    void handle_player();
    void handle_old_format_overworld_data();
    void handle_overworld();
  private:
    // The save we’re building.
    cSave* mp_save;
    // level objects for parsing
    Save_Level_ObjectList m_level_objects;
    // level spawned objects for parsing
    cSprite_List m_level_spawned_objects;
    // overworld waypoints for parsing
    Save_Overworld_WaypointList m_waypoints;
    // The file we’re parsing.
    boost::filesystem::path m_savefile;
    // The <property> results we found before the current tag.
    XmlAttributes m_current_properties;
    // Whether the old format was detected
    bool m_is_old_format;
  };

}

#endif
