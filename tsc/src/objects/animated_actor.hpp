#ifndef TSC_ANIMATED_ACTOR_HPP
#define TSC_ANIMATED_ACTOR_HPP
namespace TSC {

    /**
     * Actors that do not always show the same graphic, but rather
     * show different ones depending on the situation are instances
     * of this class or of one of its subclasses. The methods for
     * managing the multitude of textures associated with such
     * an instance comes from the cImageSet class — note this
     * class does multiple inheritance for this.
     *
     * Do not use the inherited Set_Texture() method with instances
     * of this class and its subclasses. Use Set_Image_Num() as
     * inherited from cImageSet(), as only that method allows you
     * to refer to what was defined in the image sets. Set_Image_Num()
     * recurs to Set_Texture() under the hood, but that’s an implementation
     * detail.
     */
    class cAnimatedActor: public cSpriteActor, public cImageSet
    {
    public:
        cAnimatedActor(boost::filesystem::path relative_texture_path);

        virtual void Set_Image_Set_Image(const struct ConfiguredTexture* p_new_image, bool new_startimage = false);
        virtual std::string Get_Identity(void);
    };

}
#endif
