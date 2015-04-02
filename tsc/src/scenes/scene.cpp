#include "../core/global_basic.hpp"
#include "../scripting/scriptable_object.hpp"
#include "../objects/actor.hpp"
#include "scene.hpp"

using namespace TSC;

cScene::~cScene()
{
    //
}

void cScene::Handle_Event(sf::Event& evt)
{
    //
}

void cScene::Update()
{
    //
}

void cScene::Draw(sf::RenderWindow& stage)
{
    //
}

/**
 * Return the name of this scene. This is only used for debugging the
 * scene stack. Please return a name that identifies your cScene
 * subclass uniquely.
 */
std::string cScene::Name() const
{
    return "(Generic Scene)";
}
