Class: AudioClass
=================

* This is the
{:toc}

The `Audio` singleton, which is the sole instance of `AudioClass`,
allows you to interact with SMC’s sound system. You can play any sound
from SMC’s sound collection and any music that could also be used as a
background music in a level. Namely, paths to sound files are relative
to the SMC **sounds/** directory and music paths relative to the SMC
**music/** directory. The following table lists some examples (of
course you have to adapt the paths to your local setup):

|-----------------------+---------------------------------------+--------------------------------------|
| SMC installation path | Sound path                            | Music path                           |
|-----------------------+---------------------------------------+--------------------------------------|
|/usr/local             | /usr/local/share/smc/sounds           | /usr/local/share/smc/music           |
|-----------------------+---------------------------------------+--------------------------------------|
|C:\Program files\SMC   | C:\Program files\SMC\share\smc\sounds | C:\Program files\SMC\share\smc\music |
|-----------------------+---------------------------------------+--------------------------------------|

TODO: Check the Windows path.

So, if you want to play the star music, you first have to find where
the music file is located. So, assuming your SMC is installed at
**/usr/local**, you’d find the star music at
**/usr/local/share/smc/music/game/star.ogg**. To play it, you’d take
the path relative to **/usr/local/share/smc/music/**,
i.e. **game/star.ogg**. This is what you pass on to a method such as
[play_music](#playmusic):

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Audio:play_music("game/star.ogg")
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
{:lang="lua"}

Note that path elements are always separated with a forward slash /,
even if the native path separation on some platforms is another (such
as \ on Windows).

Instance methods
----------------

### play_sound #################################################################
    play_sound( filename [, volume [, loops [, resid ] ] ] ) → a_bool

Plays a sound.

#### Parameters

filename
: Path to the sound file to play, relative to the *sounds/*
  directory.

volume (-1)
: Volume the sound shall have. Between 0 and 100.

resid (-1)
: Special identifier to prevent a sound from being
  played while another instance of this sound is already being
  played. SMC ensures that no two sounds with the same resource
  ID are played at the same time, i.e. the running sound will
  be stopped and discarded before your sound is played. You can
  define your own IDs, but there is a number of IDs predefined
  by SMC:

  1. Maryo jump sound.
  2. Maryo wall hit sound
  3. Maryo powerdown sound
  4. Maryo ball sound
  5. Maryo death sound
  6. Fireplant, blue mushroom, ghost mushroom mushroom and feather
     sound
  7. 1-Up mushroom and moon sound
  8. Maryo Au! (not used currently)
  9. Maryo stop sound

  Specifying -1 for this parameter allows the given sound to be played
  multiple times.

#### Return value

True on success, false otherwise. Possible failure reasons include
incorrect filenames or the sound may simply have been muted by
the user in SMC’s preferences, so you probably shouldn’t give
too much on this.

### play_music #################################################################
    play_music( filename [, loops [, force [, fadein_ms = 0 ] ] ] ) → a_bool

Plays a music (optionally replacing the currently played one, if any).

#### Parameters

filename
: Name of the music file to play, relative to the **music/**
  directory.

loops (0)
: Number of times to repeat the music _after_ it has been played once,
  i.e. the number of musics played equals `1 + loops`.

force (true)
: Enforces the new music to run even if another music
  is already running (the running music is stopped and discarded).
  Note this behaviour is enabled by default, you have to explicitely
  set this to `false`.

fadein_ms (0)
: Number of milliseconds to fade the music in.

#### Return value

True on success, false otherwise. Possible failure reasons include
incorrect filenames or the music may simply have been muted by
the user in SMC’s preferences, so you probably shouldn’t give
too much on this.
