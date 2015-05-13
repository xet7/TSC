#include "../core/global_basic.hpp"
#include "../core/global_game.hpp"
#include "../core/property_helper.hpp"
#include "../scripting/scriptable_object.hpp"
#include "../core/file_parser.hpp"
#include "../video/img_set.hpp"
#include "../objects/actor.hpp"
#include "../objects/sprite_actor.hpp"
#include "../objects/animated_actor.hpp"
#include "../core/collision.hpp"
#include "level_player.hpp"

using namespace TSC;
namespace fs = boost::filesystem;

cLevel_Player::cLevel_Player()
    : cAnimatedActor()
{
    m_name = "Alex";
    m_coltype = COLTYPE_MASSIVE;
    m_gravity_accel = 2.8f;
    m_gravity_max = 25.0f;

    Add_Image_Set("small", "alex/small/walk.imgset");

    Set_Image_Set("small");
    Set_Image_Num(0, true);
}

cLevel_Player::~cLevel_Player()
{
    //
}

bool cLevel_Player::Handle_Collision(cCollision* p_collision)
{
    cAnimatedActor::Handle_Collision(p_collision);

    if (p_collision->Is_Collision_Bottom()) {
        Set_On_Ground(p_collision->Get_Collision_Sufferer());
    }

    return true;
}
