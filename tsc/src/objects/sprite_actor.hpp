#ifndef TSC_SPRITE_ACTOR_HPP
#define TSC_SPRITE_ACTOR_HPP

namespace TSC {

    /**
     * Abstract class for actors wrapping an SFML sprite.
     * You should not use this class bare. Use the cStaticActor
     * and cAnimatedActor subclasses instead.
     */
    class cSpriteActor: public cActor
    {
    public:
        cSpriteActor();
        virtual ~cSpriteActor();

        virtual void Draw(sf::RenderWindow& stage) const;
        virtual void Added_To_Level(cLevel* p_level, const unsigned long& uid);

        void Set_Dimensions(int width, int height);

    protected:
        sf::Sprite m_sprite;
    };

}

#endif
