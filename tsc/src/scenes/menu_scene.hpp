#ifndef TSC_SCENES_MENU_SCENE_HPP
#define TSC_SCENES_MENU_SCENE_HPP
namespace TSC {

    /**
     * The Scene for the game’s main menu.
     */
    class cMenuScene: public cScene
    {
    public:
        cMenuScene();
        virtual ~cMenuScene();

        virtual void Handle_Event(sf::Event& evt);
        virtual void Update();
        virtual void Draw(sf::RenderWindow& stage);
    protected:
        sf::Sprite m_menuitem_start;
        sf::Sprite m_menuitem_quit;
        sf::Texture m_menuitem_start_texture;
        sf::Texture m_menuitem_quit_texture;
    };

}
#endif
