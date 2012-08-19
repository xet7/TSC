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
(such as changing the filename). Perhaps the most interesting method
on this object is [finish()](#finish), which makes the player
immediately win a level.

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
