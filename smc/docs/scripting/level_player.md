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

### downgrade ##################################################################
    downgrade()

Hurts Maryo. Kills him if he is small.

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
