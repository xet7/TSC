Class: Flyon
============
Parent: [Enemy](enemy.html)
{: .superclass}

* This is the
{:toc}

![Flyon](graphics/flyon.png){:.enemyimg} These guys called _Flyons_
usually lurk in pipes or other dark places where you won’t see
them. They then suddenly jump out of their hiding place and if you
don’t react fast enough, they’ll get you. However, if you block the
pipe a flyon lurks in it won’t find the exit...

As with the [eato](eato.html), SMC knows currently about two kinds of
flyons, the _orange_ one (shown above) and the _blue_ one (they’re
nearly extinct currently, so don’t be surprised if you never saw
one--they’ve been in the game before 2.0). You can create new flyon
colours by creating a subdirectory under `pixmaps/enemy/flyon/`, look
into the existing directories to get an idea of the structure. In
order to use your new flyon (or just an already existing one), pass
the path to the flyon graphics directory to the [new()](#new) method.

Flyons are one of the few sprites that make use of the both kinds of
coordinates assigned to SMC sprites (the normal ones and the initial
coordinates, see [Sprite](sprite.html)). The initial coordinates
determine the position from which the flyon starts its attacks,
whereas the normal coordinates represent the position the flyon
momently is at. Setting only the latter will cause funny results, as
the initial coordinates default to (0|0), where the flyon then wants
to return to!

Class methods
-------------

### new ########################################################################
    new( direction [, image_directory ] ) → a_flyon

Creates a new flyon.

#### Parameters
direction
: The direction to look into. One of the following
  self-explanatory strings:

  * `up`
  * `down`
  * `right`
  * `left`

image_directory (`"enemy/flyon/orange/"`)
: The path to the flyon’s graphics directory, relative to the
  `pixmaps/` directory. Should terminate with a slash `/` as it’s a
  directory name.

#### Return value

The newly created flyon.

Instance methods
----------------

### get_image_dir ##############################################################
    get_image_dir() → a_string

The path to the flyon’s graphics directory, relative to
`pixmaps/`. Has a trailing slash.

### get_max_distance ###########################################################
    get_max_distance() → a_number

Returns the maximum distance the flyon may fly, in pixels.

### get_speed ##################################################################
    get_speed() → a_number

Returns the flyon’s flying speed.

### is_moving_back #############################################################
    is_moving_back() → a_bool

Checks whether the flyon currently moves backwards.

#### Return value

If so, returns `true`, otherwise returns `false`.

### set_image_dir ##############################################################
    set_image_dir( path )

Change the flyon’s graphics.

#### Parameter
path
: The graphics directory for the flyon, relative to `pixmaps/`.

### set_max_distance ###########################################################
    set_max_distance( distance )

Sets the maximum distance the flyon may fly into its looking
direction.

#### Parameter
distance
: The flying distance in pixels.

### set_speed ##################################################################
    set_speed( speed )

Sets the flying speed.

#### Parameter
speed
: The new flying speed.

### wait_time ##################################################################
    wait_time() → a_number

The time the flyon is going to wait until the next approach. This
number is calculated internally automatically and may change (most
notably it’s 0 while the flyon is jumping), so there’s no way to set
it.

#### Return value

The current waiting time. The returned number may include fractions.
