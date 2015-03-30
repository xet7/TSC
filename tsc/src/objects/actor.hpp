#ifndef TSC_ACTOR_HPP
#define TSC_ACTOR_HPP

namespace TSC {

    /**
     * Base class for everything in a level. Not actors are non-level
     * elements such as the HUD.
     *
     * This is an abstract class that is intended to be subclassed.
     */
    class cActor: public Scripting::cScriptable_Object, public sf::Sprite
    {
    public:
        cActor();
        virtual ~cActor();

        virtual void Update();
        virtual void Draw(sf::RenderWindow& stage);
    };

    /**
     * Actors that don’t move and don’t do much interaction. This
     * is notably the case for ground tiles.
     */
    class cStaticActor: public cActor
    {
    public:
        cStaticActor();
        virtual ~cStaticActor();
    };
}

#endif
