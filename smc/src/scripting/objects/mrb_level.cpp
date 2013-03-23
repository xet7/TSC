#include "../../level/level.h"
#include "mrb_level.h"

/**
 * Class: LevelClass
 *
 * `LevelClass` exposes it’s sole instance through the `Level` singleton,
 * which always points to the currently active level. It is a mostly
 * informational object allowing you to access a level’s global settings,
 * but does not permit you to change them, because this either wouldn’t
 * make much sense in the first place (why change the author name from
 * within the script?) or could even cause severe confusion for the game
 * (such as changing the filename).
 *
 * This class allows you to register handlers for two very special
 * events: The **save** and the **load** event. These events are not
 * fired during regular gameplay, but instead when the player creates a
 * new savegame (**save**) or restores an existing one (**load**). By
 * returning an MRuby hash from the **save** event handler, you can
 * advertise SMC to store it in the savegame; later, when the user loads
 * this savegame again, the hash is deserialised from the savegame and
 * passed back as an argument to the even thandler of the **load**
 * event. This way you can store information on your level from within
 * the scripting API that will persist between saves and loads of a
 * level.
 *
 * Consider this example:
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ruby
 * # Say, you have a number of switches in your
 * # level. Their state is stored inside this
 * # global table.
 * switches = {
 *   :blue  => false,
 *   :red   => false,
 *   :green => false
 * }
 *
 * # The player may activate your switches,
 * # causing the respective entry in the
 * # global `switches' table to change.
 * UIDS[114].on_touch do |collidor|
 *   switches[:red] = true if collidor:player?
 * end
 *
 * # Now, if the player jumps on your switch and
 * # then saves and reloads, the switch’s state
 * # gets lost. To prevent this, we define handlers
 * # for the `save' and `load' events that persist
 * # the state of the global `switches' table.
 * # See below to see why we cannot dump the booleans
 * # into the savegame.
 * Level.on_save do |hsh|
 *   hsh.replace(switches)
 * end
 *
 * Level.on_load do |hsh|
 *   switches.replace(hsh)
 * end
 *
 * # This way the switches will remain in their
 * # respective state even after saving/reloading
 * # a game. If you change graphics for pressed
 * # switches, you still have to do this manually
 * # in your event handlers, though.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * Please note that the complexity of the table to be stored inside the
 * savegame is fairly limited. You can store an aribtrary number of
 * entries inside it (as long as you don’t overfill the user’s hard
 * drive), but they *keys have to be strings*. No other type is allowed
 * for the sake of an easy internal handling in the C++ code. Likewise,
 * the table’s values must be *convertible* to strings, i.e. although they
 * don’t have to be strings itself, they have to be easily convertible
 * into strings. Numbers and of course strings are fine for
 * this. Nested tables are not allowed either. Furthermore, after
 * restoring the table from the savegame, both the keys *and* the values
 * will be strings.
 *
 * Internal note
 * -------------
 *
 * You will most likely neither notice nor need it, but the Lua `Level`
 * singleton actually doesn’t wrap SMC’s notion of the currently running
 * level, `pActive_Level`, but rather the pointer to the savegame
 * mechanism, `pSavegame`. This facilitates the handling of the event
 * table for levels.
 *
 * Events
 * ------
 *
 * Load
 * : Called when the user loads a savegame containing this level. The
 *   event handler gets passed a Lua table containing any values
 *   requested in the **save** event’s handler, but note both the keys
 *   and values are strings now. Do not assume your level is active when
 *   this is called, the player may be in a sublevel (however, usually
 *   this has no impact on what you want to restore, but don’t try to
 *   warp the player or things like that, it will result in undefined
 *   behaviour probably leading SMC to crash).
 *
 * Save
 * : Called when the users saves a game. The event handler should return
 *   a table containing all the values you want to preserve between level
 *   loading and saving, but please see the explanations further above
 *   regarding the limitations of this table. Do not assume your level is
 *   active when this is called, because the player may be in a sublevel
 *   (however, usually this has no impact on what you want to save).
 */

using namespace SMC;
using namespace SMC::Scripting;

// Extern
struct RClass* SMC::Scripting::p_rcLevel     = NULL;
struct mrb_data_type SMC::Scripting::rtLevel = {"Level", NULL};

/***************************************
 * Methods
 ***************************************/

static mrb_value Initialize(mrb_state* p_state,  mrb_value self)
{
	mrb_raise(p_state, MRB_NOTIMP_ERROR(p_state), "Cannot create instances of this class.");
	return self; // Not reached
}

/**
 * Method: Level#author
 *
 *   author() → a_string
 *
 * Returns the content of the level’s *Author* info field.
 */
static mrb_value Get_Author(mrb_state* p_state,  mrb_value self)
{
	return mrb_str_new_cstr(p_state, pActive_Level->m_author.c_str());
}

void SMC::Scripting::Init_Level(mrb_state* p_state)
{
  p_rcLevel = mrb_define_class(p_state, "Level", p_state->object_class);

  mrb_define_method(p_state, p_rcLevel, "initialize", Initialize, ARGS_NONE());
  mrb_define_method(p_state, p_rcLevel, "author", Get_Author, ARGS_NONE());
}
