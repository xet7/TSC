/***************************************************************************
 * hud.h
 *
 * Copyright © 2003 - 2011 Florian Richter
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

#ifndef TSC_HUD_HPP
#define TSC_HUD_HPP

#include "../objects/movingsprite.hpp"
#include "../core/obj_manager.hpp"

namespace TSC {

    /* *** *** *** *** *** *** *** cHudSprite *** *** *** *** *** *** *** *** *** *** */

    class cHudSprite : public cSprite {
    public:
        cHudSprite(cSprite_Manager* sprite_manager);
        virtual ~cHudSprite(void);

        // copy this sprite
        virtual cHudSprite* Copy(void) const;
    };

    /* *** *** *** *** *** *** *** cHud_Manager *** *** *** *** *** *** *** *** *** *** */

    class cHud_Manager : public cObject_Manager<cHudSprite> {
    public:
        cHud_Manager(cSprite_Manager* sprite_manager);
        virtual ~cHud_Manager(void);

        // Load the complete HUD
        void Load(void);
        // Unload the complete HUD
        void Unload(void);

        // Update and reload text
        void Update_Text(void);
        // Update the objects
        void Update(void);
        // Draw the objects
        void Draw(void);

        // Set the parent sprite manager
        void Set_Sprite_Manager(cSprite_Manager* sprite_manager);

        // the parent sprite manager
        cSprite_Manager* m_sprite_manager;

        typedef vector<cHudSprite*> HudSpriteList;

        // true if loaded
        bool m_loaded;
    };

    /* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

// The HUD Manager
    extern cHud_Manager* pHud_Manager;

    /* *** *** *** *** *** cMenuBackground *** *** *** *** *** *** *** *** *** *** *** *** */

    class cMenuBackground : public cHudSprite {
    public:
        cMenuBackground(cSprite_Manager* sprite_manager);
        virtual ~cMenuBackground(void);

        virtual void Draw(cSurface_Request* request = NULL);

        cGL_Surface* m_alex_head;
        cGL_Surface* m_goldpiece;

        GL_point m_rect_alex_head;
        GL_point m_rect_goldpiece;
    };

    /* *** *** *** *** *** cStatusText *** *** *** *** *** *** *** *** *** *** *** *** */

    class cStatusText {
    public:
        cStatusText();
        virtual ~cStatusText(void);

        virtual void Draw();

    protected:
        void Prepare_Text_For_SFML(const std::string&, int fontsize, Color color);

    private:
        sf::Text m_text;
        float m_x;
        float m_y;
    };

    /* *** *** *** *** *** cMiniPointsText *** *** *** *** *** *** *** *** *** *** *** *** */

    class cMiniPointsText {
    public:
        cMiniPointsText();
        virtual ~cMiniPointsText();

        virtual void Draw();

        inline void Set_Vel_Y(float vely) { m_vely = vely; }
        inline void Enable() { m_disabled = false; }
        inline void Disable() { m_disabled = true; }
        inline bool Is_Disabled(){ return m_disabled; }

        inline sf::Text& Get_Text(){ return m_text; }
        inline float Get_Vel_Y(){ return m_vely; }

    private:
        float m_vely;
        sf::Text m_text;
        bool m_disabled;
    };

    /* *** *** *** *** *** cPlayerPoints *** *** *** *** *** *** *** *** *** *** *** *** */

    class cPlayerPoints : public cStatusText {
    public:
        cPlayerPoints();
        virtual ~cPlayerPoints(void);

        void Set_Points(long points);
        void Add_Points(unsigned int points, float x = 0.0f, float y = 0.0f, std::string strtext = "", const Color& color = static_cast<Uint8>(255), bool allow_multiplier = 0);

        // removes all point texts
        void Clear(void);

        virtual void Draw();

        std::vector<cMiniPointsText*> m_points_objects;
    };

    /* *** *** *** *** *** cGoldDisplay *** *** *** *** *** *** *** *** *** *** *** *** */

    class cGoldDisplay : public cStatusText {
    public:
        cGoldDisplay(cSprite_Manager* sprite_manager);
        virtual ~cGoldDisplay(void);

        void Set_Gold(int gold);
        void Add_Gold(int gold);

        virtual void Draw(cSurface_Request* request = NULL);
    };

    /* *** *** *** *** *** cLiveDisplay *** *** *** *** *** *** *** *** *** *** *** *** */

    class cLiveDisplay : public cStatusText {
    public:
        cLiveDisplay(cSprite_Manager* sprite_manager);
        virtual ~cLiveDisplay(void);

        void Set_Lives(int lives);
        void Add_Lives(int lives);

        virtual void Draw(cSurface_Request* request = NULL);
    };

    /* *** *** *** *** *** cTimeDisplay *** *** *** *** *** *** *** *** *** *** *** *** */

    class cTimeDisplay : public cStatusText {
    public:
        cTimeDisplay(cSprite_Manager* sprite_manager);
        virtual ~cTimeDisplay(void);

        // update
        virtual void Update(void);
        // draw
        virtual void Draw(cSurface_Request* request = NULL);

        // Set time
        void Set_Time(Uint32 milliseconds);

        // reset
        void Reset(void);

        char m_text[50];
        Uint32 m_last_update_seconds;
        Uint32 m_milliseconds;
    };

    /* *** *** *** *** *** cInfoMessage *** *** *** *** *** *** *** *** *** *** *** */

    class cInfoMessage : public cStatusText {
    public:
        cInfoMessage(cSprite_Manager* sprite_manager);
        virtual ~cInfoMessage(void);

        // Update
        virtual void Update(void);
        // draw
        virtual void Draw(cSurface_Request* request = NULL);

        // Set text and display it fading out slowly.
        void Set_Text(const std::string& text);
        std::string Get_Text();

    private:
        std::string m_text;
        float m_alpha;
        float m_display_time;
        cMovingSprite* m_background;
    };

    /* *** *** *** *** *** cItemBox *** *** *** *** *** *** *** *** *** *** *** *** */

    class cItemBox : public cStatusText {
    public:
        cItemBox(cSprite_Manager* sprite_manager);
        virtual ~cItemBox(void);

        // Set the parent sprite manager
        virtual void Set_Sprite_Manager(cSprite_Manager* sprite_manager);

        // update
        virtual void Update(void);
        // draw
        virtual void Draw(cSurface_Request* request = NULL);

        /* Set the item
        * sound : if set the box sound is played
        */
        void Set_Item(SpriteType item_type, bool sound = 1);
        // Activates the itembox
        void Request_Item(void);
        // push the item back to the itembox
        void Push_back(void);

        void Reset(void);

        /* The current Item
         * uses the Item defines
         */
        SpriteType m_item_id;

        // alpha effect
        float m_item_counter;
        // alpha effect mod
        bool m_item_counter_mod;

        // itembox color
        Color m_box_color;

        // stored item
        cMovingSprite* m_item;
    };

    /* *** *** *** *** *** cDebugDisplay *** *** *** *** *** *** *** *** *** *** *** *** */

    class cDebugDisplay : public cStatusText {
    public:
        cDebugDisplay(cSprite_Manager* sprite_manager);
        virtual ~cDebugDisplay(void);

        // update
        virtual void Update(void);
        // draw
        virtual void Draw(cSurface_Request* request = NULL);
        // draw the frames per second info
        void Draw_fps(void);
        // draw the debug mode info
        void Draw_Debug_Mode(void);
        // draw the performance debug mode info
        void Draw_Performance_Debug_Mode(void);

        // set the debug text to display
        void Set_Text(const std::string& ntext, float display_time = speedfactor_fps * 2.0f);

        // display text
        std::string m_text, m_text_old;
        // text counter
        float m_counter;

        // CEGUI debug text
        CEGUI::Window* m_window_debug_text, *m_text_debug_text;

        // last game mode
        GameMode m_game_mode_last;
        // last level filename
        boost::filesystem::path m_level_old;
        // last object counters
        int m_obj_counter, m_pass_counter, m_mass_counter, m_enemy_counter, m_active_counter;
        // sprites
        typedef vector<cHudSprite*> HudSpriteList;
        HudSpriteList m_sprites;
    };

    /* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

// The HUD
    extern cPlayerPoints* pHud_Points;
    extern cDebugDisplay* pHud_Debug;
    extern cGoldDisplay* pHud_Goldpieces;
    extern cLiveDisplay* pHud_Lives;
    extern cTimeDisplay* pHud_Time;
    extern cInfoMessage* pHud_Infomessage;
    extern cItemBox* pHud_Itembox;

    /* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

} // namespace TSC

#endif
