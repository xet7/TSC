// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#include <CEGUIWindowManager.h>
#include <elements/CEGUIMultiLineEditbox.h>
#include <elements/CEGUIScrollbar.h>
#include "../script.h"
#include "../../core/main.h"
#include "../../core/property_helper.h"
#include "../../core/game_core.h"
#include "../../core/framerate.h"
#include "../../level/level_player.h"
#include "../../input/keyboard.h"
#include "../../input/joystick.h"
#include "../../user/preferences.h"
#include "../../audio/audio.h"
#include "../events/event.h"
#include "l_message.h"

/************************************************
 * Most of the code in this file is heavily
 * inspired by the code in objects/text_box.cpp.
 * Look there if you don’t get everything.
 ***********************************************/

using namespace SMC;

// Width and height of message windows displayed by this Lua class
static unsigned int message_window_width	= 300;
static unsigned int message_window_height	= 200;

// Helper method prototypes
static void Show_Message_Exclusively(CEGUI::MultiLineEditbox* p_edit);

/***************************************
 * Class methods
 ***************************************/

static int Allocate(lua_State* p_state)
{	
	if (!lua_istable(p_state, 1))
		return luaL_error(p_state, "No class table given.");
	std::string text = luaL_checkstring(p_state, 2);

	/* Generate unique window titles (a new window may be created while the
	 * old one is still existing, causing CEGUI to complain about existing
	 * window titles) */
	static int id			= 0; // function-static
	std::string wintitle	= "LuaMessage";
	id++;
	wintitle.append(int_to_string(id));

	CEGUI::WindowManager& wm			= CEGUI::WindowManager::getSingleton();
	CEGUI::MultiLineEditbox** pp_edit	= (CEGUI::MultiLineEditbox**) lua_newuserdata(p_state, sizeof(CEGUI::MultiLineEditbox*));
	CEGUI::MultiLineEditbox* p_edit		= static_cast<CEGUI::MultiLineEditbox*>(wm.createWindow("TaharezLook/MultiLineEditbox", wintitle));
	*pp_edit							= p_edit;

	LuaWrap::InternalC::set_imethod_table(p_state, 1); // Attach instance methods

	// Add to main window
	pGuiSystem->getGUISheet()->addChildWindow(p_edit);

	// Set properties common over multiple displays
	p_edit->setAlwaysOnTop(true);
	p_edit->setWidth(CEGUI::UDim(0, message_window_width * global_upscalex));
	p_edit->setHeight(CEGUI::UDim(0, message_window_height * global_upscaley));
	p_edit->setText((const CEGUI::utf8*)text.c_str());

	// Invisible by default
	p_edit->hide();

	return 1;
}

static int Finalize(lua_State* p_state)
{
	CEGUI::WindowManager& wm		= CEGUI::WindowManager::getSingleton();
	CEGUI::MultiLineEditbox* p_edit	= *LuaWrap::check<CEGUI::MultiLineEditbox*>(p_state, 1);
	wm.destroyWindow(p_edit);
	return 0;
}

/***************************************
 * Instance methods
 ***************************************/

static int Get_Text(lua_State* p_state)
{
	CEGUI::MultiLineEditbox* p_edit = *LuaWrap::check<CEGUI::MultiLineEditbox*>(p_state, 1);
	lua_pushstring(p_state, p_edit->getText().c_str());
	return 1;
}

static int Set_Text(lua_State* p_state)
{
	CEGUI::MultiLineEditbox* p_edit = *LuaWrap::check<CEGUI::MultiLineEditbox*>(p_state, 1);
	std::string text = luaL_checkstring(p_state, 2);
	p_edit->setText((const CEGUI::utf8*)text.c_str());
	return 0;
}

static int Append(lua_State* p_state)
{
	CEGUI::MultiLineEditbox* p_edit = *LuaWrap::check<CEGUI::MultiLineEditbox*>(p_state, 1);
	std::string text = luaL_checkstring(p_state, 2);
	p_edit->setText(p_edit->getText() + (const CEGUI::utf8*)text.c_str());
	return 0;
}

static int Message_Display(lua_State* p_state)
{
	CEGUI::MultiLineEditbox* p_edit	= *LuaWrap::check<CEGUI::MultiLineEditbox*>(p_state, 1);

	// Get coordinates for displaying centered above Maryo
	float x = pLevel_Player->m_pos_x - 0.5f * message_window_width;
	float y = pLevel_Player->m_pos_y - message_window_height - 5;

	// Convert the level coordinates to the window
	// coordinates CEGUI understands
	p_edit->setXPosition(CEGUI::UDim(0, (x - pActive_Camera->m_x) * global_upscalex));
	p_edit->setYPosition(CEGUI::UDim(0, (y - pActive_Camera->m_y) * global_upscaley));

	// Start mini-eventloop that only cares about this window
	Show_Message_Exclusively(p_edit);

	return 0;
}

/***************************************
 * Binding
 ***************************************/

static luaL_Reg Methods[] = {
	{"append", Append},
	{"display", Message_Display},
	{"get_text", Get_Text},
	{"set_text", Set_Text},
	{NULL, NULL}
};

void Script::Open_Message(lua_State* p_state)
{
	LuaWrap::register_class<CEGUI::MultiLineEditbox>(	p_state,
														"Message",
														Methods,
														NULL,
														Allocate,
														Finalize);
}

/***************************************
 * Helpers methods
 ***************************************/

/**
 * When displaying a message, the game is paused. Events have
 * to be processed nevertheless (most importantly for recognising
 * when the user wants to close the message box). This method
 * encapsulates a mini-eventloop that does exactly this. It shows
 * then given editbox window, loops, and when the user exits the
 * editbox, hides the window and returns.
 */
static void Show_Message_Exclusively(CEGUI::MultiLineEditbox* p_edit)
{
	// Show the window
	p_edit->show();

	// Event loop
	bool display = true;
	while(display){
		while(SDL_PollEvent(&input_event)){
			switch(input_event.type){
			case SDL_KEYDOWN:
				pKeyboard->m_keys[input_event.key.keysym.sym] = 1;

				if (input_event.key.keysym.sym == pPreferences->m_key_action
					|| input_event.key.keysym.sym == SDLK_ESCAPE
					|| input_event.key.keysym.sym == SDLK_RETURN
					|| input_event.key.keysym.sym == SDLK_SPACE){
					display = false;
					goto terminate;
				}
				else if (input_event.key.keysym.sym == pPreferences->m_key_right
						 || input_event.key.keysym.sym == pPreferences->m_key_left)
					pKeyboard->Key_Down(input_event.key.keysym.sym);
				
				break;
			case SDL_KEYUP:
				pKeyboard->m_keys[input_event.key.keysym.sym] = 0;

				if (input_event.key.keysym.sym == pPreferences->m_key_right
				   || input_event.key.keysym.sym == pPreferences->m_key_left)
					pKeyboard->Key_Up(input_event.key.keysym.sym);
				break;
			case SDL_JOYBUTTONDOWN:
				pJoystick->Set_Button(input_event.jbutton.button, 1);
				if (input_event.jbutton.button == pPreferences->m_joy_button_action
				   || input_event.jbutton.button == pPreferences->m_joy_button_exit){
					display = false;
					goto terminate;
				}
				break;
			case SDL_JOYBUTTONUP:
				pJoystick->Set_Button(input_event.jbutton.button, 0);
				break;
			case SDL_JOYHATMOTION:
				pJoystick->Handle_Hat(&input_event);
				goto terminate;
			case SDL_JOYAXISMOTION:
				pJoystick->Handle_Motion(&input_event);
				goto terminate;
			} // switch
		} // while(SDL_PollEvent(&input_event))

	terminate:
		Uint8* keys = SDL_GetKeyState(NULL);
		Sint16 joy_ver_axis = 0;

		if (pPreferences->m_joy_enabled)
			joy_ver_axis = SDL_JoystickGetAxis(pJoystick->m_joystick, pPreferences->m_joy_axis_ver);

		// Handle up and down scrolling
		if (keys[pPreferences->m_key_down] || joy_ver_axis > pPreferences->m_joy_axis_threshold)
			p_edit->getVertScrollbar()->setScrollPosition( p_edit->getVertScrollbar()->getScrollPosition() + ( p_edit->getVertScrollbar()->getStepSize() * 0.25f * pFramerate->m_speed_factor ) );
		if (keys[pPreferences->m_key_up] || joy_ver_axis < -pPreferences->m_joy_axis_threshold)
			p_edit->getVertScrollbar()->setScrollPosition( p_edit->getVertScrollbar()->getScrollPosition() - ( p_edit->getVertScrollbar()->getStepSize() * 0.25f * pFramerate->m_speed_factor ) );

		// Update what needs to be updated
		pAudio->Update();
		Draw_Game();
		pVideo->Render();
		pFramerate->Update();
	} // while(display)

	// Hide the window
	p_edit->hide();
}
