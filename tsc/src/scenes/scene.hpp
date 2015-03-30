#ifndef TSC_SCENE_HPP
#define TSC_SCENE_HPP

namespace TSC {

    /**
     * Base class for all scenes in the game. A scene defines what
     * is currently happening, it has control of the entire scene
     * and the entire event handling stack (except for the global
     * event handlers defined in the cSpriteManager directly). In
     * each scene, there are actors (cActor instances) who play
     * some role in this scene.
     *
     * This is an abstract class intended to be subclasses. By default,
     * it just calls `Update()` and `Draw()` on each of the actors in
     * this scene.
     */
    class cScene
    {
    public:
        virtual ~cScene();
        virtual void Handle_Event(sf::Event& evt);
        virtual void Update();
        virtual void Draw(sf::RenderWindow& stage);

        inline std::vector<cActor*>::iterator begin_actors(){return m_actors.begin();}
        inline std::vector<cActor*>::iterator end_actors(){return m_actors.end();}

    protected:
        std::vector<cActor*> m_actors;
    };
}

#endif
