Class: LevelPlayer
==================
Parent: [AnimatedSprite](animated_sprite.html)
{: .superclass}

* This is the
{:toc}

The sole instance of this class, the singleton `Player`, represents
Maryo himself. Naturally you can’t instanciate this class (SMC isn’t a
multiplayer game), but otherwise this class is your interface to doing
all kinds of evil things with Maryo. You should, however, be careful,
because the powerful methods exposed by this class allow you to
control nearly every aspect of Maryo--if you exaggerate, the player
will get annoyed, probably stopping to play your level.

This class’ documentation uses two words that you’re better off not
mixing up:

Maryo (or just "the level player")
: The sprite of Maryo walking around and jumping on enemies.

Player
: The actual user sitting in front of some kind of monitor. Don’t
  confuse him with Maryo, because a) he will probably get angry, and b)
  you’ll get funny meanings on sentences like "the player presses the
  _jump_ key". The only exception to this rule is the `Player`
  variable in Lua, which obviously represents Maryo, not the guy
  playing the game.

Events
------

Downgrade
: Whenever Maryo gets hit (but not killed), this event is triggered.
  The event handler gets passed Maryo’s current downgrade count
  (which is always 1) and Maryo’s maximum downgrade count (which
  is always 2). As you can see, the arguments passed are not really
  useful and are just there for symmetry with some enemies’
  _Downgrade_ event handlers.

Jump
: This event is issued when the Maryo does a valid jump, i.e. the
  player presses the _Jump_ key and Maryo is currently in a state that
  actually allows him to jump. The event is triggered immediately
  before starting the jump.

Shoot
: Fired when Maryo executes a valid shoot, either fireball or
  iceball. As with the _Jump_ event, this is only triggered when the
  player presses the _Shoot_ key and Maryo is currently in a state
  that allows him to shoot. Likewise, the event is triggered just
  prior to the actual shot. The event handler gets passed either the
  string `"ice"` when the player fired an iceball, or `"fire"` when it
  was a fireball.

Instance methods
----------------

### add_gold ###################################################################
    add_gold( num )    → a_number
    add_waffles( num ) → a_number

Add to the player’s current amount of gold/waffles.

#### Parameter
num
: The number of gold pieces/waffles to add. If Maryo’s resulting
  amount of gold pieces/waffles (i.e. the current amount plus `num`)
  is greater than 100, Maryo gains a life and 100 is subtractacted
  from the resulting amount. This process is repeated until the total
  resulting amount of gold pieces/waffles is not greater than 100.

#### Return value
The new amount of gold pieces/waffles (after the 100-rule described
above has been applied as often as necessary).

### add_lives ##################################################################
    add_lives( lives ) → a_number

Add to the player’s current number of lives.

#### Parameter
lives
: The lives to add. This number may be negative, but note that setting
  lives to 0 or less doesn’t kill the player immediately as this
  number is only checked when the player gets killed by some other
  force.

### add_points #################################################################
    add_points( points ) → a_number

Adds more points to the amount of points the player already has.

#### Parameter
points
: The number of points to add.

#### Return value
The new number of points.

### add_waffles ################################################################
_Alias for [add_gold()](#addgold)._

### downgrade ##################################################################
    downgrade()

Hurts Maryo. Kills him if he is small.

### get_gold ###################################################################
    get_gold()  → a_number
    get_waffles → a_number

The current amount of gold pieces/waffles Maryo has collected so
far. This is always smaller than 100.

### get_points #################################################################
    get_points() → a_number

Returns the number of points the player currently has.

### get_type ###################################################################
    get_type() → a_string

Returns Maryo’s current type. See [set_type()](#settype) for a list of
possible strings to be returned.

### get_waffles ################################################################
_Alias for [get_gold()](#getgold)._

### jump #######################################################################
    jump( deaccel )

Makes Maryo jump.

#### Parameter
deaccel
: Negative acceleration to apply, i.e. defines how high Maryo will
  jump. Note that this isn’t necessarily the height in pixels as the
  force of gravity will be applied to the value while jumping.

### kill #######################################################################
    kill()

Immediately sends Maryo to heaven (or to hell; it depends).

### set_gold ###################################################################
    set_gold( num )
    set_waffles( num )

Reset the number of collected gold pieces/waffles to the given value.

#### Parameter
num
: The new number of gold pieces/waffles. This value obeys the same
  100-rule as the parameter to [add_gold()](#addgold).

### set_lives ##################################################################
    set_lives( lives )

Reset Maryo’s number of lives to the given value.

#### Parameter
lives
: The new number of lives. This number may be negative, but note that
  setting lives to 0 or less doesn’t kill the player immediately as
  this number is only checked when the player gets killed by some other
  force.

### set_points #################################################################
    set_points( points )

Reset the player’s points to the given value. You probably don’t want
to do this.

#### Parameter
points
: The new number of points.

### set_type ###################################################################
    set_type( type )

Applies a powerup/powerdown to the level player. Note this method
bypasses any Maryo state checks, i.e. you can directly apply `ice` to
small Maryo or force Fire Maryo back to Normal Big Maryo by applying
`big`. This check bypassing is the reason why you shouldn’t use this
method for downgrading or killing the player; there might however be
situations in which calling this method is more appropriate.

Using this method never affects the rescue item (that one shown on top
of the screen in the box).

#### Parameter
type
: The powerup or powerdown to apply. One of the following strings:

  `dead`
   : Please use the [kill()](#kill) method instead.

  `small`
   : Please use the [downgrade()](#downgrade) method instead.

  `big`
  : Apply the normal mushroom.

  `fire`
  : Apply the fireplant.

  `ice`
  : Apply the ice mushroom.

  `ghost`
  : Apply the ghost mushroom.

  Specifying an invalid type causes an error.

### set_waffles ################################################################
_Alias for [set_gold()](#setgold)._
