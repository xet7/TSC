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
        /**
         * Type defining the main Z layer positions. The `m_pos_z` of an
         * actor is added to this. You should only access representatives
         * of this type via the `ZLAYER_*` constants.
         */
        typedef float ZLayer;

        const ZLayer ZLAYER_PASSIVE      = 0.01f;     //< Minimum Z position for passive objects.
        const ZLayer ZLAYER_HALFMASSIVE  = 0.04f;     //< Minimum Z position for half-massive objects.
        const ZLayer ZLAYER_CLIMBABLE    = 0.05f;     //< Minimum Z position for climbable objects.
        const ZLayer ZLAYER_MASSIVE      = 0.08f;     //< Minimum Z position for massive objects.
        const ZLayer ZLAYER_PLAYER       = 0.0999f;   //< Z position of the level player.
        const ZLayer ZLAYER_FRONTPASSIVE = 0.10f;     //< Minimum Z position for front-passive objects.
        const ZLayer ZLAYER_POS_DELTA    = 0.000001f; //< Minimum Z step.

        cSpriteActor(boost::filesystem::path texture_path);
        virtual ~cSpriteActor();

        float Z() const;

        virtual void Draw(sf::RenderWindow& stage) const;

    private:
        sf::Sprite m_sprite;
        float m_pos_z;
        ZLayer m_z_layer;
    };

}

#endif
