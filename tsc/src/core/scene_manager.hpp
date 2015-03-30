#ifndef TSC_SCENE_MANAGER_HPP
#define TSC_SCENE_MANAGER_HPP

namespace TSC {

    /**
     * Main class for managing the happenings in the game. Each state
     * the game may has is represented by a scene that encapsulates
     * all the information required for it.
     *
     * The scenes are managed by a stack, where the scene on top of
     * the stack is the active (current) scene. By placing a new scene
     * on top of the stack, the game switches to it; by removing the
     * top scene from the stack, the game returns to the previous
     * scene.
     */
    class cSceneManager
    {
    public:
        cSceneManager();

        void Push_Scene(cScene* p_scene);
        cScene* Pop_Scene();

        void Play(sf::RenderWindow& stage);
        inline cScene& Current_Scene(){return *m_scenes_stack.top();}
        inline void End_Play(){m_end_play = true;}
    private:
        bool Handle_Global_Event(sf::Event& evt);

        std::stack<cScene*> m_scenes_stack;
        bool m_end_play;
    };

}

#endif
