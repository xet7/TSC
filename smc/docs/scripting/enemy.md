Class: Enemy
============
Parent: [AnimatedSprite](animated_sprite.html)
{: .superclass}

* This is the
{:toc}

_Enemies_ are the little guys that hang around all over the world of
SMC and try to hinder you in many different ways. This is the base
class for all enemies and defines the methods that are available for
every enemy, regardless of its type.

Currently, this class is not instanciatable.

Events
------

Die
: This event gets triggered when the enemy dies. The event handler
  doesn’t get passed any argument.

Instance methods
----------------

### disable_fire_resistance ####################################################
    disable_fire_resistance()

Makes this enemy vulnerable to fire. See also
[enable_fire_resistance()](#enablefireresistance) and
[is_fire_resistant()](#isfireresistant).

### enable_fire_resistance #####################################################
    enable_fire_resistance()

Makes this enemy resistant to fire. See also
[disable_fire_resistance()](#disablefireresistance) and
[is_fire_resistant()](#isfireresistant).

### get_kill_points ############################################################
    get_kill_points() → a_number

Returns the number of points the player gains after killing this
enemy.

### get_kill_sound #############################################################
    get_kill_sound() → a_string

Returns the filename of the sound to play when the enemy gets killed,
relative to the `sounds/` directory.

### is_fire_resistant ##########################################################
    is_fire_resistant() → a_bool

Checks whether this enemy is resistant to fire. See also
[enable_fire_resistance()](#enablefireresistante) and
[disable_fire_resistance](#disablefireresistance).

#### Return value

If the check is successful, returns `true`, `false` otherwise.

### kill #######################################################################
    kill()

Immediately kills this enemy. No points are given to
the player (i.e. it is as if the enemy fell into an abyss
or something like that).

Causes a subsequent _Die_ event.

See also: kill_with_points().

### kill_with_points ###########################################################
    kill_with_points()

Immediately kills this enemy. Points are given to the
player. Also plays the dying sound for this enemy.

Causes a subsequent _Die_ event.

See also: kill().

### set_kill_points ############################################################
    set_kill_points( points )

Sets the number of points the player gains after killing this
enemy. Note kill points are not applied if the enemy dies due to
something else than player interaction, e.g. by falling into an
abyss.

#### Parameters
points
: Number of points to add.

### set_kill_sound #############################################################
    set_kill_sound( path )

Sets the sound to play when the enemy gets killed.

#### Parameters
path
: The path of the sound file to play. Relative to the `sounds/`
  directory.
