#ifndef TSC_STATIC_ACTOR_HPP
#define TSC_STATIC_ACTOR_HPP

namespace TSC {

    /**
     * Visible actors that don’t move and don’t do much
     * interaction. This is notably the case for ground tiles.
     */
    class cStaticActor: public cActor
    {
    public:
        cStaticActor(boost::filesystem::path texture_path);
        virtual ~cStaticActor();

        virtual void Draw(sf::RenderWindow& stage) const;

    private:
        sf::Sprite m_sprite;
    };

}

#endif
