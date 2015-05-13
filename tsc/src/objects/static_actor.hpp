#ifndef TSC_STATIC_ACTOR_HPP
#define TSC_STATIC_ACTOR_HPP
namespace TSC {

    /**
     * Nonmoving actors whose sprite always shows an unanimated single
     * image. This is most notably the case for ground tiles.
     */
    class cStaticActor: public cSpriteActor
    {
    public:
        cStaticActor();
        cStaticActor(boost::filesystem::path relative_texture_path);
        virtual ~cStaticActor();

        virtual void Draw(sf::RenderWindow& stage) const;
        virtual void Added_To_Level(cLevel* p_level, const unsigned long& uid);

        void Set_Texture(boost::filesystem::path relative_texture_path);
    protected:
        boost::filesystem::path m_rel_texture_path;
    };

}
#endif
