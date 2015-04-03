#ifndef TSC_STATIC_ACTOR_HPP
#define TSC_STATIC_ACTOR_HPP

namespace TSC {

    /**
     * Visible actors that don’t move and don’t do much
     * interaction. This is notably the case for ground tiles.
     */
    class cSpriteActor: public cActor
    {
    public:
        cSpriteActor(boost::filesystem::path texture_path);
        virtual ~cSpriteActor();

        virtual void Draw(sf::RenderWindow& stage) const;

    private:
        sf::Sprite m_sprite;
    };

}

#endif
