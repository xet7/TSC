#ifndef TSC_ACTOR_HPP
#define TSC_ACTOR_HPP

namespace TSC {

    /**
     * Base class for everything in a level. Not actors are non-level
     * elements such as the HUD.
     *
     * This is an abstract class that is intended to be subclassed.
     */
    class cActor: public Scripting::cScriptable_Object
    {
    public:
        cActor();
        virtual ~cActor();

        virtual void Update();
        virtual void Draw(sf::RenderWindow& stage);

        void Set_Dimensions(int width, int height);

        inline void Set_Collision_Rect(sf::FloatRect rect){m_collision_rect = rect;}
        inline const sf::FloatRect& Get_Collision_Rect(){return m_collision_rect;}

        inline void Set_Name(std::string name){m_name = name;}
        inline std::string Get_Name(){return m_name;}

        inline sf::Sprite& Get_Sprite() {return m_sprite;}
    protected:
        sf::Sprite m_sprite;
        sf::FloatRect m_collision_rect;
        std::string m_name;
    };

}

#endif
