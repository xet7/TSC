Class: LevelClass
=================

* This is the
{:toc}

`LevelClass` exposes it’s sole instance through the `Level` singleton,
which always points to the currently active level. It is a mostly
informational object allowing you to access a level’s global settings,
but does not permit you to change them, because this either wouldn’t
make much sense in the first place (why change the author name from
within the script?) or could even cause severe confusion for the game
(such as changing the filename).

This class allows you to register handlers for two very special
events: The **save** and the **load** event. These events are not
fired during regular gameplay, but instead when the player creates a
new savegame (**save**) or restores an existing one (**load**). By
returning a Lua table from the **save** event handler, you can
advertise SMC to store it in the savegame; later, when the user loads
this savegame again, the table is deserialised from the savegame and
passed back as an argument to the even thandler of the **load**
event. This way you can store information on your level from within
the scripting API that will persist between saves and loads of a
level.

Consider this example:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
-- Say, you have a number of switches in your
-- level. Their state is stored inside this
-- global table.
switches = {
  blue  = false,
  red   = false,
  green = false
}

-- The player may activate your switches,
-- causing the respective entry in the
-- global `switches' table to change.
function activate_red_switch(collidor)
  if collidor:is_player() then
    switches["red"] = true
  end
end
UIDS[114]:on_touch(activate_red_switch) -- 114 is your switch

-- Now, if the player jumps on your switch and
-- then saves and reloads, the switch’s state
-- gets lost. To prevent this, we define handlers
-- for the `save' and `load' events that persist
-- the state of the global `switches' table.
-- See below to see why we cannot dump the booleans
-- into the savegame.
function save_switch_states()
  local tbl = {}
  for k, v in pairs(tbl) do
    if v then
      tbl[k] = "true"
    else
      tbl[k] = "false"
    end
  end
  return tbl;
end

function restore_switch_states(tbl)
  for k, v in pairs(tbl) do
    if v == "true" then
      switches[k] = true
    else
      switches[k] = false
    end
  end
end

Level:on_save(save_switch_states)
Level:on_load(restore_switch_states)

-- This way the switches will remain in their
-- respective state even after saving/reloading
-- a game. If you change graphics for pressed
-- switches, you still have to do this manually
-- in your event handlers, though.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
{:lang="lua"}

Please note that the complexity of the table to be stored inside the
savegame is fairly limited. You can store an aribtrary number of
entries inside it (as long as you don’t overfill the user’s hard
drive), but they *keys have to be strings*. No other type is allowed
for the sake of an easy internal handling in the C++ code. Likewise,
the table’s values must be *convertible* to strings, i.e. although they
don’t have to be strings itself, they have to be easily convertible
into strings. Numbers and of course strings are fine for
this. Nested tables are not allowed either. Furthermore, after
restoring the table from the savegame, both the keys *and* the values
will be strings.

Internal note
-------------

You will most likely neither notice nor need it, but the Lua `Level`
singleton actually doesn’t wrap SMC’s notion of the currently running
level, `pActive_Level`, but rather the pointer to the savegame
mechanism, `pSavegame`. This facilitates the handling of the event
table for levels.

Events
------

Load
: Called when the user loads a savegame containing this level. The
  event handler gets passed a Lua table containing any values
  requested in the **save** event’s handler, but note both the keys
  and values are strings now. Do not assume your level is active when
  this is called, the player may be in a sublevel (however, usually
  this has no impact on what you want to restore, but don’t try to
  warp the player or things like that, it will result in undefined
  behaviour probably leading SMC to crash).

Save
: Called when the users saves a game. The event handler should return
  a table containing all the values you want to preserve between level
  loading and saving, but please see the explanations further above
  regarding the limitations of this table. Do not assume your level is
  active when this is called, because the player may be in a sublevel
  (however, usually this has no impact on what you want to save).

Instance methods
----------------

### get_author #################################################################
    get_author() → a_string

Returns the content of the level’s *Author* info field.

### get_description ############################################################
    get_description() → a_string

Returns the content of the level’s *Description* info field,.

### get_difficulty #############################################################
    get_difficulty() → a_number

Returns the difficulty information of the level, reaching from 0
(undefined) over 1 (very easy) to 100 ((mostly) uncompletable).

### get_engine_version #########################################################
    get_engine_version() → a_number

Returns the SMC engine version used to create the level.

### get_filename ###############################################################
    get_filename() → a_string

Returns the level’s filename.

### get_music_filename #########################################################
    get_music_filename( [ format [, with_ext ] ] ) → a_string

Returns the default level music’s filename.

#### Parameters

format ("remove_nothing")
: Specifies the format of the path string
  returned. "remove_complete_dir" returns only the bare filename,
  "remove_music_dir" returns a path relative to the `music/` directory
  and "remove_nothing" returns an absolute path.

with_ext (false)
: If set, the returned path will not have a file extension.

### get_next_level_filename ####################################################
    get_next_level_filename() → a_string

If a new level shall automatically be loaded when this level
completes, this returns the filename of the target level. Otherwise
the return value is undefined, but most likely an empty string.

### get_script #################################################################
    get_script() → a_string

Returns the Lua code associated with this level.

### finish #####################################################################
    finish( [win_music] )

The player immediately wins the level and the game resumes to the
world overview, advancing to the next level point. If the level was
loaded using the level menu directly (and hence there is no
overworld), returns to the level menu.

#### Parameters

win_music (false)
: If set, plays the level win music.
