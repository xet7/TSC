Class: Sprite
=============

A _Sprite_ is the most generic object available in the Lua
API. Anything shown on the screen is somehow a sprite, and the methods
defined in this class are therefore available to nearly all objects
exposed to the Lua API.

The scripting API makes use of two different kinds of sprites (which
both are represented by the `Sprite` class), _externally_ created
sprites and _internally_ created ones. The latter are those you create
from inside the Lua code (by calling the `new()` method of the
`Sprite` class or one of its subclasses). The former are those created
by regular use of the SMC editor, i.e. anything already available
before even the first piece of your Lua script has been run. These
sprites are automatically assigned a so-called "unique identifier",
UID for short, that (as the name implies) uniquely identifies a
certain sprite from all the others. The UID is displayed inside the
SMC editor when hovering the mouse cursor over an object. Internally
created sprites are not assigned an UID automatically, but you can do
so by passing a value you choose to the `new()` method. All sprites
with UIDs are available anywhere in the Lua script by indexing the
`Sprite` class table (or any subclass class table) with the UID. So,
if you have a sprite with UID 38 in the editor, you can find out its X
coordinate like this:

    Sprite[38]:get_x()

Events
------

Touch
: This event is fired when the sprite collides with another
  sprite. Note that such a collision actually creates two Touch
  events, one for each sprite. This may be useful if you don’t want to
  use a long `if-elseif-elseif-elseif...` construct inside an event
  handler choosing an action depending on the collided sprite.

  The event handler gets passed an instance of this class representing
  the other collision "partner".

Class methods
-------------

### (indexing) #################################################################
    [ index ] → ?

The `Sprite` class overrides the normal Lua `__index` metamethod. In
addition to doing normal lookups, it allows you to reference a Sprite
instance by just knowing it’s UID. So, if you index the `Sprite` table
with a number, you’ll get back an instance of class `Sprite` (or `nil`
if there is no sprite with this UID); otherwise, the table will behave
as any other ordinary Lua table.

Note that the `Sprite` subclasses behave likewise, but `Sprite` won’t
automatically wrap the object into the necessary subclass. Therefore,
if you want an enemy, use `Enemy[<ID>]` to get an instance of class
`Enemy`. `Sprite[<ID>]` will only give you instances of class
`Sprite`, probably without the methods you intended to call.

#### Parameters
index
: The index to look up. If it’s a number, behaves as described
  above. Otherwise, behaves like a normal Lua table.

#### Return value

In case of a number passed, a `Sprite` instance or `nil`, otherwise the result
of a normal Lua table lookup.

### new ########################################################################
    new( [ image_path [, x_pos [, y_pos [, uid ] ] ] ] ) → a_sprite

Creates a new sprite. Note the spride is hidden by default, you need
to explicitely call [show()](#show) on it.

#### Parameters

image_path (nil)
: Path to image for this sprite. Relative to the **pixmaps/**
  directory.

x_pos (nil)
: X position to place the sprite at.
y_pos (nil)
: Y position to place the sprite at.
uid (nil)
: The UID for this sprite. This is useful if you want to reference
  your sprite in another context, e.g. when you create a sprite in a
  signal handler and want to reference it in another signal
  handler. The sprite is then treated the same way all the other
  sprites are and is available through the `[]` call.

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
: The event handler, i.e. the function to write that shall be called
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


### set_x ######################################################################
    set_x( val )

Sets a new X coordinate.

### set_y ######################################################################
    set_y( val )

Sets a new Y coordinate.


### show #######################################################################
    show()

Makes a sprite visible. See also [hide()](#hide).


### wrap #######################################################################
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
