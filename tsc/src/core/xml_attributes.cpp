/***************************************************************************
 * xml_attributes.cpp - XML attributes editing
 *
 * Copyright © 2013 - 2014 The TSC Contributors
 ***************************************************************************/
/*
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "global_basic.hpp"
#include "global_game.hpp"
#include "errors.hpp"
#include "property_helper.hpp"
#include "xml_attributes.hpp"
#include "../scripting/scriptable_object.hpp"
#include "../objects/actor.hpp"
#include "../scenes/scene.hpp"
#include "scene_manager.hpp"
#include "filesystem/resource_manager.hpp"
#include "../video/img_manager.hpp"
#include "filesystem/package_manager.hpp"
#include "i18n.hpp"
#include "../user/preferences.hpp"
#include "tsc_app.hpp"

using namespace TSC;

void XmlAttributes::relocate_image(const std::string& filename_old, const std::string& filename_new, const std::string& attribute_name /* = "image" */)
{
    std::string current_value = (*this)[attribute_name];
    std::string filename_old_full = path_to_utf8(gp_app->Get_ResourceManager().Get_Game_Pixmaps_Directory() / filename_old);

    if (current_value == filename_old || current_value == filename_old_full)
        (*this)[attribute_name] = filename_new;
}

bool XmlAttributes::exists(const std::string& key)
{
    if (count(key) > 0)
        return true;
    else
        return false;
}

void TSC::Add_Property(xmlpp::Element* p_element, const Glib::ustring& name, const Glib::ustring& value)
{
    xmlpp::Element* p_propnode = p_element->add_child("property");
    p_propnode->set_attribute("name", name);
    p_propnode->set_attribute("value", value);
}

void TSC::Replace_Property(xmlpp::Element* p_element, const Glib::ustring& name, const Glib::ustring& value)
{
    // Determine if the property exists first
    xmlpp::Node::NodeList children = p_element->get_children("property");

    for(xmlpp::Node::NodeList::iterator it = children.begin(); it != children.end(); ++it) {
        xmlpp::Element* element = dynamic_cast<xmlpp::Element*>(*it);

        if(element && element->get_attribute_value("name") == name) {
            element->set_attribute("value", value);
            return;
        }
    }

    // No match found
    TSC::Add_Property(p_element, name, value);
}

