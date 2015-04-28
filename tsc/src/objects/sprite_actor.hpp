#ifndef TSC_STATIC_ACTOR_HPP
#define TSC_STATIC_ACTOR_HPP

namespace TSC {

    /**
     * Visible actors that don’t move and don’t do much
     * interaction. This is notably the case for ground tiles.
     */
    class cSpriteActor: public cActor, public cImageSet
    {
    public:
        cSpriteActor(boost::filesystem::path relative_texture_path);
        virtual ~cSpriteActor();

        virtual void Draw(sf::RenderWindow& stage) const;
        virtual void Added_To_Level(cLevel* p_level, const unsigned long& uid);

        void Set_Dimensions(int width, int height);
        void Set_Texture(boost::filesystem::path relative_texture_path);

        virtual void Set_Image_Set_Image(const struct ConfiguredTexture* p_new_image, bool new_startimage = false);
        virtual std::string Get_Identity(void);

    private:
        sf::Sprite m_sprite;
        boost::filesystem::path m_rel_texture_path;
    };

}

#endif
