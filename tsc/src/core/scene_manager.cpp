#include "global_basic.hpp"
#include "../scripting/scriptable_object.hpp"
#include "../objects/actor.hpp"
#include "../scenes/scene.hpp"
#include "scene_manager.hpp"

using namespace TSC;

cSceneManager::cSceneManager()
{
    m_end_play = false;
}

/**
 * Pushes a scene onto the scenes stack, making it the current scene
 * in the next iteration of the main loop.
 */
void cSceneManager::Push_Scene(cScene* p_scene)
{
    debug_print("Pushing scene: %s\n", p_scene->Name().c_str());
    m_scenes_stack.push(p_scene);
}

/**
 * Pops a scene from the scenes stack and returns it. You’ll have
 * to `delete` the pointer yourself if you want to free it.
 * If the scene stack is empty after popping the current scene,
 * the game will automatically be ended in the next iteration of
 * the main loop.
 */
cScene* cSceneManager::Pop_Scene()
{
    cScene* p_scene = m_scenes_stack.top();
    m_scenes_stack.pop();

    debug_print("Popping scene: %s\n", p_scene->Name().c_str());

    if (m_scenes_stack.empty())
        m_end_play = true;

    return p_scene;
}

/**
 * Game main loop. This method polls all events from SFML, clears the screen,
 * asks the current scene to update itself, then to draw itself, and then
 * displays the result. When the game loop has been requested to terminate
 * (e.g. by End_Play()), deletes (i.e. frees) all scenes currently in the
 * scene stack before it returns and closes the SFML window.
 */
void cSceneManager::Play(sf::RenderWindow& stage)
{
    // Main loop
    while (!m_end_play) {
        // Measure time needed per mainloop iteration (= per frame)
        m_mainloop_elapsed_time = m_game_clock.restart();
        CEGUI::System::getSingleton().injectTimePulse(m_mainloop_elapsed_time.asSeconds());

        // Get scene on top of the stack.
        cScene* p_current_scene = m_scenes_stack.top();

        /* Event handling. Poll all events from SFML, and then ask
         * the global event handler to handle them. If the global
         * event handler doesn’t process them, hand them to the
         * current scene’s local event handler. */
        sf::Event evt;
        while (stage.pollEvent(evt)) {
            if (!Handle_Global_Event(evt)) {
                p_current_scene->Handle_Event(evt);
            }
        }

        // Clear screen
        stage.clear();

        // Moving and other updates
        p_current_scene->Update();

        // Draw the current scene into the back buffer
        p_current_scene->Draw(stage);

        // Render CEGUI on top of it. CEGUI must always use the default view
        // without any zooming or other things applied! It’s directly tied
        // to the window, and has nothing to do with a level or so.
        stage.setView(stage.getDefaultView());
        CEGUI::System::getSingleton().renderGUI();

        // Show it
        stage.display();
    }

    // Wipe all scenes that still exist
    while (!m_scenes_stack.empty())
        delete Pop_Scene();

    // Applause!
    stage.close();
}

/**
 * Global events that apply to any scene. Returns true if this
 * method handled the event, false otherwise.
 *
 * Note that this handler especially forwards all relevant events
 * to CEGUI. If CEGUI reacts on an event, this method will return
 * true, otherwise false.
 *
 *  It does react on some other things as well, though.
 */
bool cSceneManager::Handle_Global_Event(sf::Event& evt)
{
    CEGUI::System& cegui = CEGUI::System::getSingleton();

    switch (evt.type) {
    case sf::Event::Closed: // Window received QUIT event
        m_end_play = true;
        return true;
    case sf::Event::MouseMoved:
        return cegui.injectMousePosition(evt.mouseMove.x, evt.mouseMove.y);
    case sf::Event::MouseButtonPressed:
        switch(evt.mouseButton.button) {
        case sf::Mouse::Left:
            return cegui.injectMouseButtonDown(CEGUI::LeftButton);
        case sf::Mouse::Middle:
            return cegui.injectMouseButtonDown(CEGUI::MiddleButton);
        case sf::Mouse::Right:
            return cegui.injectMouseButtonDown(CEGUI::RightButton);
        default:
            return false;
        }
    case sf::Event::MouseButtonReleased:
        switch(evt.mouseButton.button) {
        case sf::Mouse::Left:
            return cegui.injectMouseButtonUp(CEGUI::LeftButton);
        case sf::Mouse::Middle:
            return cegui.injectMouseButtonUp(CEGUI::MiddleButton);
        case sf::Mouse::Right:
            return cegui.injectMouseButtonUp(CEGUI::RightButton);
        default:
            return false;
        }
    case sf::Event::TextEntered:
        return cegui.injectChar(evt.text.unicode);
    case sf::Event::KeyPressed:
        return cegui.injectKeyDown(SFMLKey2CEGUIKey(evt.key.code));
    case sf::Event::KeyReleased:
        return cegui.injectKeyUp(SFMLKey2CEGUIKey(evt.key.code));
    default:
        return false;
    }
}

unsigned int cSceneManager::SFMLKey2CEGUIKey(const sf::Keyboard::Key& key)
{
    switch(key) {
    case sf::Keyboard::Return:
        return CEGUI::Key::Return;
    case sf::Keyboard::BackSpace:
        return CEGUI::Key::Backspace;
    case sf::Keyboard::Tab:
        return CEGUI::Key::Tab;
    case sf::Keyboard::Escape:
        return CEGUI::Key::Escape;
    case sf::Keyboard::LControl:
        return CEGUI::Key::LeftControl;
    case sf::Keyboard::RControl:
        return CEGUI::Key::RightControl;
    case sf::Keyboard::LAlt:
        return CEGUI::Key::LeftAlt;
    case sf::Keyboard::RAlt:
        return CEGUI::Key::RightAlt;
    case sf::Keyboard::LShift:
        return CEGUI::Key::LeftShift;
    case sf::Keyboard::RShift:
        return CEGUI::Key::RightShift;
    case sf::Keyboard::Up:
        return CEGUI::Key::ArrowUp;
    case sf::Keyboard::Down:
        return CEGUI::Key::ArrowDown;
    case sf::Keyboard::Right:
        return CEGUI::Key::ArrowRight;
    case sf::Keyboard::Left:
        return CEGUI::Key::ArrowLeft;
    case sf::Keyboard::PageUp:
        return CEGUI::Key::PageUp;
    case sf::Keyboard::PageDown:
        return CEGUI::Key::PageDown;
    default:
        return 0; // Ignore
    }
}
