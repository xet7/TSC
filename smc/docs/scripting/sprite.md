Class: Sprite
=============

* This is the
{:toc}

A _Sprite_ is the most generic object available in the Lua
API. Anything shown on the screen is somehow a sprite, and the methods
defined in this class are therefore available to nearly all objects
exposed to the Lua API.

All sprites created by the regular SMC editor can be references by
indexing the global `UIDS` table, see
[Unique Identifiers](index.html#unique-identifiers-uids) for more
information on this topic.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Sprite[38]:get_x()
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
{:lang="lua"}

About coordinates
-----------------

Instances of `Sprite` and its subclasses have two kinds of
coordinates, the normal ones (usually called just "coordinates") and
the starting position’s coordinates (usually called "initial
coordinates"). Most sprites don’t really care about the initial
coordinates, but some instances of subclasses of `Sprite` do, e.g. the
[flyon](flyon.html) remembers its starting position, i.e. where to
return after jumping out, in its initial coordinates.

Events
------

Touch
: This event is fired when the sprite collides with another
  sprite. Note that such a collision actually creates two Touch
  events, one for each sprite. This may be useful if you don’t want to
  use a long `if-elseif-elseif-elseif...` construct inside an event
  handler choosing an action depending on the collided sprite.

  The event handler gets passed an instance of this class (or one of
  its subclasses) representing the other collision "partner".

Class methods
-------------

### new ########################################################################
    new( [ image_path [, uid ] ] ) → a_sprite

Creates a new sprite. Note the spride is hidden by default, you need
to explicitely call [show()](#show) on it.

#### Parameters

image_path (nil)
: Path to image for this sprite. Relative to the **pixmaps/**
  directory.

uid (nil)
: The UID for this sprite. This is useful if you want to reference
  your sprite in another context, e.g. when you create a sprite in a
  event handler and want to reference it in another event
  handler. The sprite is then treated the same way all the other
  sprites are and is available through the `UIDS` table.

#### Return value

The newly created instance.

Instance methods
----------------

### get_uid ####################################################################
    get_uid() → a_number

Returns the UID for the sprite.

#### Return value

The sprite’s UID or -1 if it doesn’t have one assigned.

### get_collision_rect #########################################################
    get_collision_rect() → x, y, width, height

The sprite’s collision rectangle. See also [get_rect()](#getrect)

### get_rect ###################################################################
    get_rect() → x, y, width, height

The sprite’s full image rectangle. See also [get_collision_rect()](#getcollisionrect).

### get_start_x ################################################################
    get_start_x() → a_number

Returns the sprite’s initial X coordinate.

### get_start_y ################################################################
    get_start_y() → a_number

Returns the sprite’s initial Y coordinate.

### get_x ######################################################################
    get_x() → a_number

The current X coordinate.

### get_y ######################################################################
    get_y() → a_number

The current Y coordinate.


### get_z ######################################################################
    get_z() → a_number

Returns the current Z coordinate. Note you cannot set the Z
coordinate.

### hide #######################################################################
    hide()

Makes a sprite invisible. See also [show()](#show).

### is_player ##################################################################
    is_player() → a_bool

Checks whether this sprite is the player.

#### Return value

If this sprite is the player, returns `true`. Otherwise, returns
`false`.

### pos ########################################################################
    pos() → x, y

Returns the sprite’s current X and Y coordinates.

### register ###################################################################
    register( evtname, handler )

Generic event handler registration, useful if you cannot provide the
name of the event you want to register for statically. Usually you
won’t need this method, it’s called internally whenever you call one
of the `on_*` methods.

#### Parameters

evtname
: The name of the event to register for, downcased. For example, if
  you want to register for the Touch event, pass `"touch"`.

handler
: The event handler, i.e. the function that shall be called
  when the event is triggered.

### set_massive_type ###########################################################
    set_massive_type( type )

Set the massivity of a sprite.

#### Parameters
type
: One of the following strings. Their meaning is identical to the one
  in the SMC editor.

  * `"passive"`
  * `"front_passive"` or `"frontpassive"`
  * `"massive"`
  * `"half_massive"` or `"halfmassive"`
  * `"climbable"`

  Invalid types will cause an error.

### set_start_x ################################################################
    set_start_x( val )

Sets the sprite’s initial X coordinate.

#### Parameter
val
: The X coordinate in pixels.

### set_start_y ################################################################
    set_start_y( val )

Sets the sprite’s initial Y coordinate.

#### Parameter
val
: The Y coordinate in pixels.

### set_x ######################################################################
    set_x( val )

Sets a new X coordinate.

### set_y ######################################################################
    set_y( val )

Sets a new Y coordinate.

### show #######################################################################
    show()

Makes a sprite visible. See also [hide()](#hide).

### start_at ###################################################################
    start_at( xpos, ypos )

Sets both the initial X and Y coordinates at once.

#### Parameters
xpos
: The initial X coordinate in pixels.

ypos
: The initial Y coordinate in pixels.

### start_pos ##################################################################
    start_pos() → xpos, ypos

Returns the initial coordinates for this sprite.

#### Return value

The initial coordinates, both in pixels.

### warp #######################################################################
    warp( new_x, new_y )

Warp the sprite somewhere. Note you are responsible for ensuring the
coordinates are valid, this method behaves exactly as a level entry
(i.e. doesn’t check coordinate validness).

You can easily get the coordinates by moving around the cursor in
the SMC level editor and hovering over object placed near the
location where you want to warp to.

#### Parameters
x
: The new X coordinate.

y
: The new Y coordinate.
