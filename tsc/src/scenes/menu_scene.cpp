using namespace TSC;

cMenuScene::cMenuScene()
{
    sf::Texture& menu_start_texture = gp_app->Get_Image_Manager().Get_Texture(utf8_to_path(gp_app->Get_Game_Pixmap("menu/start.png")));
    sf::Texture& menu_quit_texture  = gp_app->Get_Image_Manager().Get_Texture(utf8_to_path(gp_app->Get_Game_Pixmap("menu/quit.png")));

    m_menuitem_start.setTexture(menu_start_texture);
    m_menuitem_quit.setTexture(menu_quit_texture);

    m_menuitem_start.setPosition(sf::Vector2f(100, 100));
    m_menuitem_quit.setPosition(sf::Vector2f(100, 300));
}

cMenuScene::~cMenuScene()
{
    //
}

void cMenuScene::Handle_Event(sf::Event& evt)
{
    cScene::Handle_Event();
}

void cMenuScene::Update()
{
    cScene::Update();
}

void cMenuScene::Draw(sf::RenderWindow& stage)
{
    cScene::Draw(stage);

    m_menuitem_start.draw(stage);
    m_menuitem_quit.draw(stage);
}
