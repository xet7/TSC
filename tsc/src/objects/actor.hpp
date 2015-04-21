#ifndef TSC_ACTOR_HPP
#define TSC_ACTOR_HPP

namespace TSC {

    /**
     * Base class for everything in a level. Not actors are non-level
     * elements such as the HUD.
     *
     * This is an abstract class that is intended to be subclassed.
     * Actors are invisible by default, thus only defined by their
     * collision rectangle. If you want a visible actor, you have
     * to use some drawable entity inside your class, e.g. an sf::Sprite.
     * See the implementation of cStaticActor for an example of this.
     *
     * Each actor has a collision rectangle that gets transformed when
     * you make use of SFML’s sf::Transformable interface. Use
     * Get_Transformed_Collision_Rect() to obtain the resulting
     * collision rectangle; to retrieve the unmodified rectangle,
     * use Get_Collision_Rect(). Note that setting the collision
     * rectangle with Set_Collision_Rect() always sets the collision
     * rectangle prior to any transformation, so you can’t e.g. manually
     * set the collision rectangle of a rotated sprite.
     *
     * Note this class inherits from [sf::Transformable](http://www.sfml-dev.org/documentation/2.2/classsf_1_1Transformable.php).
     */
    class cActor: public Scripting::cScriptable_Object, public sf::Transformable
    {
    public:
        /**
         * Type defining the main Z layer positions. The `m_pos_z` of an
         * actor is added to this. You should only access representatives
         * of this type via the `ZLAYER_*` constants. See cActor::Z() for
         * a thorough explanation of Z coordinate handling.
         */
        typedef float ZLayer;

        const ZLayer ZLAYER_PASSIVE      = 0.01f;     //< Minimum Z position for passive objects.
        const ZLayer ZLAYER_HALFMASSIVE  = 0.04f;     //< Minimum Z position for half-massive objects.
        const ZLayer ZLAYER_CLIMBABLE    = 0.05f;     //< Minimum Z position for climbable objects.
        const ZLayer ZLAYER_MASSIVE      = 0.08f;     //< Minimum Z position for massive objects.
        const ZLayer ZLAYER_PLAYER       = 0.0999f;   //< Z position of the level player.
        const ZLayer ZLAYER_FRONTPASSIVE = 0.10f;     //< Minimum Z position for front-passive objects.
        const ZLayer ZLAYER_POS_DELTA    = 0.000001f; //< Minimum Z step.

        /**
         * Determines how the actor behaves in the collision detection
         * mechanism.
         **/
        enum CollisionType {
            COLTYPE_MASSIVE = 1,
            COLTYPE_PASSIVE,
            COLTYPE_ENEMY,
            COLTYPE_ACTIVE,
            COLTYPE_ANIM,
            COLTYPE_PLAYER,
            COLTYPE_LAVA
        };

        cActor();
        virtual ~cActor();

        void Do_Update();
        virtual void Draw(sf::RenderWindow& stage) const;
        virtual void Added_To_Level(cLevel* p_level, const unsigned long& uid);
        virtual bool Handle_Collision(cCollision* p_collision);

        void Set_Collision_Rect(sf::FloatRect rect);
        inline const sf::FloatRect& Get_Collision_Rect() const {return m_collision_rect;}

        sf::FloatRect Get_Transformed_Collision_Rect() const;
        bool Does_Collide(const sf::FloatRect& other_rect) const;
        bool Does_Collide(const sf::Vector2f& other_point) const;
        bool Does_Collide(const cActor& other_actor) const;

        inline void Set_Collision_Type(enum CollisionType coltype){m_coltype = coltype;}
        inline enum CollisionType Get_Collision_Type() const {return m_coltype;}

        inline void Set_Name(std::string name){m_name = name;}
        inline std::string Get_Name() const {return m_name;}

        inline const unsigned long& Get_UID() const {return m_uid;}

        void Accelerate_X(const float& deltax, bool real = false);
        void Accelerate_Y(const float& deltay, bool real = false);
        void Accelerate_XY(const float& deltax, const float& deltay, bool real = false);

        float Z() const;
        //protected:
        virtual void Update();
        void Update_Gravity();
        void Update_Position();

        cLevel* mp_level;
        unsigned long m_uid;

        sf::FloatRect m_collision_rect;
        std::string m_name;

        sf::RectangleShape m_debug_colrect_shape;

        float m_gravity_factor;   //< Mass simulation factor.
        cActor* mp_ground_object; //< Do we stand on something, and if so, on what?
        sf::Vector2f m_velocity;  //< Velocity in → and ↓ direction.

        enum CollisionType m_coltype;
        float m_pos_z;
        ZLayer m_z_layer;
    };

}

#endif
