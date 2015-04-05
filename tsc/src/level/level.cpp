#include "../core/bintree.hpp"
#include "../core/global_basic.hpp"
#include "../core/global_game.hpp"
#include "../core/property_helper.hpp"
#include "../scripting/scriptable_object.hpp"
#include "../objects/actor.hpp"
#include "../core/collision.hpp"
#include "level.hpp"

using namespace TSC;
namespace fs = boost::filesystem;

/**
 * Load a level from a file rather than creating a new one. The
 * level is not active; you have to add it to the cLevelScene
 * manually. Consider using the convenience overload
 * cLevelScene::Add_Level() that accepts a string level name, which
 * does all of this for you.
 *
 * \param levelfile
 * File to load the level from. This will not be touched, the file
 * will be opened exactly as specified; use cPackage_Manager if you
 * want to search for a level’s file path.
 *
 * \returns The loaded level.
 */
cLevel* cLevel::Load_From_File(fs::path levelfile)
{
    // TODO
    return NULL;
}

#ifdef _DEBUG
cLevel* cLevel::Construct_Debugging_Level()
{
    cLevel* p_level = new cLevel();
    p_level->m_levelfile = utf8_to_path("/tmp/debugging.tsclvl");

    p_level->Sort_Z_Elements();
    return p_level;
}
#endif

/**
 * Create a new and empty level. Does NOT load levels from level files,
 * use the Load_From_File() static member function for that. Use
 * cPackage_Manager() to find out the correct saving path.
 *
 * \param levelfile
 * File to save the level to. This will be overwritten if it exists.
 *
 * \returns The new cLevel instance.
 */
cLevel::cLevel(fs::path levelfile)
    : m_collisions(0, NULL) // 0 is reserved and guaranteed to not be taken
{
    Init();
}

/**
 * This is a private constructor that is used under the hood by
 * Load_From_File() to create a level that is not yet associated
 * with a file (the public constructor overwrites files).
 */
cLevel::cLevel()
    : m_collisions(0, NULL) // 0 is reserved and guaranteed to not be taken
{
    Init();
}

cLevel::~cLevel()
{
    std::vector<sf::Sprite*>::iterator bgiter;
    for(bgiter=m_backgrounds.begin(); bgiter != m_backgrounds.end(); bgiter++)
        delete *bgiter;

    std::vector<cActor*>::iterator actiter;
    for(actiter=m_actors.begin(); actiter != m_actors.end(); actiter++)
        delete *actiter;
}

/**
 * Common basic initialisation for all constructors.
 */
void cLevel::Init()
{
    m_engine_version = 47; // TODO: Should be in the configuration header; currently in global_game.hpp
    m_player_startpos.x = 100;
    m_player_startpos.y = 100;

    m_camera_limits.left = 0;
    m_camera_limits.top = 0;
    m_camera_limits.width = 4000;
    m_camera_limits.height = 1000;

    m_last_max_uid = 0;
}

void cLevel::Update()
{
    // Update the invisible actors
    std::vector<cActor*>::iterator actiter;
    for(actiter=m_actors.begin(); actiter != m_actors.end(); actiter++)
        (*actiter)->Do_Update();
}

void cLevel::Draw(sf::RenderWindow& stage) const
{
    // 1. Draw backgrounds
    std::vector<sf::Sprite*>::const_iterator bgiter;
    for(bgiter=m_backgrounds.begin(); bgiter != m_backgrounds.end(); bgiter++)
        stage.draw(**bgiter);

    // 2. Draw the actors in the level.
    std::vector<cActor*>::const_iterator actiter;
    for(actiter=m_actors.begin(); actiter != m_actors.end(); actiter++)
        (*actiter)->Draw(stage);
}

/**
 * Sort the actors and the backgrounds by their Z coordinates.
 *
 * \notice This is a very performance-intense operation that may
 * completely reorder the two arrays. Do not call this more often
 * than necessary, especially not once per frame!.
 *
 * TODO: Background are not yet sorted by this.
 */
void cLevel::Sort_Z_Elements()
{
    std::sort(m_actors.begin(), m_actors.end(), [] (const cActor* p_a, const cActor* p_b) {
            return p_a->Z() < p_b->Z();
        });
}

/**
 * Add the given collision to the list of collisions for this frame, if required.
 * For performance reasons, collision checking only happens when an actor moves;
 * thus, the collision object will always be created with the `causer` property
 * of the collision set to the moving actor and the `sufferer` property set to
 * the standing actor. If the standing actor really doesn’t move, this means
 * that only one collision is generated, and this will be added directly to
 * the list of collisions by this method.
 *
 * However, problems arise when both actors move. Then each of the two moving
 * actors will receive one collision object, with the actors being exactly
 * inverse. These technically two collisions however are logically only a
 * single one and should not be evaluated twice (this would for example cause
 * a Krush to be immediately killed rather than downgraded when you jump on it).
 * That’s what this method enforces:
 *
 * 1. It checks if the sufferer of the collision you want to add is already
 *    in the list of collisions. That’s not enough (could be a collision
 *    with another object), but it’s required.
 * 2. Check if the collision sufferer of the found collision is the same
 *    actor as the collision causer of the collision you’re about to add.
 *    If this is the case, you’re trying to add the described inversely
 *    mirrored collision that should not be counted. In that case, this
 *    method will do nothing, especially not add the collision to the
 *    list of collisions to evaluate.
 *
 * The equality check is done by utilising the UIDs of the actors, and
 * the collision list is a binary tree using the UIDs as keys, thus
 * these actions should be rather performant.
 *
 * \param[in] p_collision
 * The cCollision instance to add to the list of collisions to evaluate.
 * This will be memory-managed by this class afterwards, so don’t delete it;
 * this is true even if the collision is not needed (then it’s deleted
 * immediately).
 *
 * \remark This method assumes you’re only adding collisions from
 * the causer’s view of things. If you don’t, the algorithm described
 * above will fail.
 */
void cLevel::Add_Collision_If_Required(cCollision* p_collision)
{
    const unsigned long& myuid    = p_collision->Get_Collision_Causer().Get_UID();
    const unsigned long& otheruid = p_collision->Get_Collision_Sufferer().Get_UID();

    const cCollision* p_coll = m_collisions.Fetch(otheruid);
    if (p_coll && p_coll->Get_Collision_Sufferer().Get_UID() == myuid) {
        delete p_collision;
        return;
    }

    m_collisions.Insert(new Bintree<cCollision>(myuid, p_collision));
}

/**
 * Check all possible collisions on the given actor. This is
 * an expensive operation as it iterates the entire actor list.
 * When a collision is found, it is added via Add_Collision_If_Required()
 * to the list of collisions that are to be handled.
 *
 * This method does not honour the type of the actor you pass in. It
 * is assumed that you check if it is something like COLTYPE_PASSIVE
 * before calling this method (to prevent the extra calling overhead).
 *
 * \param[in] actor
 * The actor to check for collisions on. It is assumed to be the
 * collision causer (i.e. the moving object, not the standing one).
 */
void cLevel::Check_Collisions_For_Actor(cActor& actor)
{
    std::vector<cActor*>::const_iterator iter;
    for(iter=m_actors.begin(); iter != m_actors.end(); iter++) {
        cActor& other = **iter;

        if (other.Get_Collision_Type() != cActor::COLTYPE_PASSIVE && actor.Does_Collide(other)) {
            Add_Collision_If_Required(new cCollision(actor, other));
            // MRuby Touch event is not fired here, that’s fired when the collisions are handled.
        }
    }
}
