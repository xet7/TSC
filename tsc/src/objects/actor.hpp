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
        cActor();
        virtual ~cActor();

        virtual void Update();
        virtual void Draw(sf::RenderWindow& stage) const;

        inline void Set_Collision_Rect(sf::FloatRect rect){m_collision_rect = rect;}
        inline const sf::FloatRect& Get_Collision_Rect() const {return m_collision_rect;}

        sf::FloatRect Get_Transformed_Collision_Rect() const;
        bool Does_Collide(const sf::FloatRect& other_rect) const;
        bool Does_Collide(const sf::Vector2f& other_point) const;
        bool Does_Collide(const cActor& other_actor) const;

        inline void Set_Name(std::string name){m_name = name;}
        inline std::string Get_Name() const {return m_name;}
    protected:

        sf::FloatRect m_collision_rect;
        std::string m_name;
    };

}

#endif
