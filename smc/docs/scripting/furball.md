Class: Furball
==============
Parent: [Enemy](enemy.html)
{: .superclass}

* This is the
{:toc}

![Furball](graphics/furball.png){:.enemyimg} The _Furball_ is the most dangerous
enemy in the world of Secret Maryo Chronicles. If you ever see one,
be _really_ careful! They may develop secret powers you were never
aware of!

The `Furball` class manages all three kinds of furballs that may
appear in SMC, i.e. the brown (shown above), the blue and the black
furball, where the black furball is the Furball Boss. SMC identifies
and distinguishes the different furball types solely based on the
color, so think twice before changing it afterwards with
[set_color()](#setcolor).

Events
------

Downgrade
: The downgrade event is fired when the furball was hit, but not
  killed. Currently this is only triggered in case of the Furball
  Boss. The event handler gets passed the current downgrade count and
  the maximum downgrade count, i.e. the downgrade count at which the
  furball (boss) will die.

Class methods
-------------

### new ########################################################################
    new( direction [, color = "brown" ] ) → a_furball

Creates a new Furball.

#### Parameters
direction
: The direction to look into, either `"left"` or `"right"`.

color ("brown")
: One of the following three strings:

  `"brown"`
  : Normal furball.

  `"blue"`
  : More ice-resistant furball

  `"black"`
  : Furball Boss.

  Passing an invalid color causes an error.

#### Return value

The newly created instance. Note the furball is at an invalid position
right now, you have to call [set_x()](sprite.html#setx) and
[set_y()](sprite.html#sety) on it.

Instance methods
----------------

### disable_level_ends_if_killed ###############################################
    disable_level_ends_if_killed()

Deactivates automatic level ending when this furball
is killed. If you call this on a furball that isn’t
a boss (i.e. its color isn’t `"black"`), raises an error.

### does_level_end_if_killed ###################################################
    does_level_end_if_killed() → a_bool

For a normal, i.e. non-black, furball always returns `false`.
Otherwise, checks if defeating this boss furball causes
the level to end and if so, returns `true`, else returns
`false`.

### enable_level_ends_if_killed ################################################
    enable_level_ends_if_killed()

Activates automatic level ending when this furball
is killed. If you call this on a furball that isn’t
a boss (i.e. its color isn’t `"black"`), raises an error.

### get_color ##################################################################
    get_color() → a_string

Gets the color of this furball.

#### Return value

See [set_color()](#setcolor) for the list of possible strings that
might get returned.

### get_max_downgrade_count ####################################################
    get_max_downgrade_count() → a_number

Returns the number of times this furball boss has already been
downgraded. If this is not a furball boss, raises an error.

### is_boss ####################################################################
    is_boss() → a_boolean

Checks whether this furball is the giant, black, and incredibly
horrible FURBALL BOSS!

#### Return value

...if so, hurries to return `true`, otherwise returns `false`.

### set_color ##################################################################
    set_color( color )

Change a Furball’s appeareance. Note that calling this also resets any
damage made to this enemy.

#### Parameter
color
: One of the following strings:

  `"brown"`
  : The normal furball.

  `"blue"`
  : The more ice-resistant furball.

  `"black"`
  : The Giant Furball Boss.

  Passing an invalid string causes an error.

### set_max_downgrade_count ####################################################
    set_max_downgrade_count( count )

Sets the number of downgrades Maryo has to give this furball boss
before it dies. If this is not a furball boss, raises an error.
