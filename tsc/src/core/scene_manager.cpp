#include "global_basic.hpp"
#include "scene_manager.hpp"

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
 */
bool cSceneManager::Handle_Global_Event(sf::Event& evt)
{
    switch (evt.type) {
    case sf::Event::Closed: // Window received QUIT event
        m_end_play = true;
        return true;
    default:
        return false;
    }
}
