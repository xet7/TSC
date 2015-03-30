#include "global_basic.hpp"
#include "scene.hpp"

using namespace TSC;

cScene::~cScene()
{
    std::vector<Actor*>::iterator iter;
    for(iter=m_actors.begin(); iter != m_actors.end(); iter++) {
        delete *iter;
    }
}

void cScene::Handle_Event(sf::Event& evt)
{
    //
}

void cScene::Update()
{
    std::vector<Actor*>::iterator iter;
    for(iter=m_actors.begin(); iter != m_actors.end(); iter++) {
        (*iter)->Update();
    }
}

void cScene::Draw(sf::RenderWindow& stage)
{
    std::vector<Actor*>::iterator iter;
    for(iter=m_actors.begin(); iter != m_actors.end(); iter++) {
        (*iter)->Draw();
    }
}
