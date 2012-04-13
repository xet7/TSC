Class: MovingSprite
===================
Parent: [Sprite](sprite.html)
{: .superclass}

Everything that is moving on the screen is considered by SMC to be a
_MovingSprite_. It is the superclass of most on-screen objects and as
such the methods defined here are available to most other objects,
e.g. the [Player](player.html) or [enemies](enemy.html).

This class may not be instanciated directly.

Instance methods
----------------

### accelarate #################################################################
    accelerate( xadd, yadd )

Add to both the horizontal and the vertical velocity at once.

#### Parameters
xadd
: What to add to the horizontal velocity. May include fractions.

yadd
: What to add to the vertical velocity. May include fractions.

### accelerate_x ###############################################################
    accelerate_x( val )

Add to the current horizontal velocity.

#### Parameter
val
: The value to add. May include fractions.

### accelerate_y ###############################################################
    accelerate_y( val )

Add to the current vertical velocity.

#### Parameter
val
: The value to add. May include fractions.

### get_direction  ##############################################################
    get_direction() → a_string

The direction the sprite is "looking" in as a string.

#### Return value

This method may return one of the following, self-explanatory strings:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
"undefined"
"left"
"right"
"up"
"down"
"up_left"
"up_right"
"down_left"
"down_right"
"left_up"
"left_down"
"horizontal"
"vertical"
"all"
"first"
"last"
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Not all of them are supported by all moving sprites (most only support
`left` and `right`), but usually you can guess which ones are
supported by looking at the images a sprite may use. The last five
directions are a bit special, namely `horizontal`, `vertical` and
`all` may be returned for objects which support "looking" into more
than one direction (e.g. a static enemy may return `all`), and `first`
and `last` can only be returned by waypoints on the world map, where
scripting isn’t supported yet.

### get_velocity ###############################################################
    get_velocity() → xvel, yvel

Get both the horizontal and the vertical velocity.

#### Return value

The horizontal velocity (`xvel`) and the vertical velocity
(`yvel`). Both values may include fractions.

### get_velocity_x #############################################################
    get_velocity_x() → a_number

Returns the current horizontal velocity; the return value may include
fractions.

### get_velocity_y #############################################################
    get_velocity_y() → a_number

Returns the current vertical velocity; the return value may include
fractions.

### set_direction  ##############################################################
    set_direction( dir )

Set the "looking" direction of the sprite.

#### Parameter

dir
: One of the looking directions supported by this sprite. See
[get_direction()](#getdirection) for a list of possible values.

### set_velocity ###############################################################
    set_velocity( xvel, yvel )

Set both the horizontal and vertical velocity at once.

#### Parameters
xvel
: The new horizontal velocity. May include fractions.

yvel
: The new vertical velocity. May include fractions.

### set_velocity_x #############################################################
    set_velocity_x( vel )

Set the horizontal velocity.

#### Parameter

val
: The new velocity. May include fractions.

### set_velocity_y #############################################################
    set_velocity_y( vel )

Set the vertical velocity.

#### Parameter

val
: The new velocity. May include fractions.
