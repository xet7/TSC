Class: Gee
==============
Parent: [Enemy](enemy.html)
{: .superclass}

* This is the
{:toc}

![Gee](graphics/gee.png){:.enemyimg} _Gees_ are flying enemies that
 may move around or stay at some place. They are not really dangerous,
 but sometimes you can jump onto one and reach some hidden place that
 was inaccesible before!

There are three different kinds of gees, identified by their
color. The mapping is as follows:

yellow
: This is the electro gee (shown above).

red
: This is the lava gee. Note it’s usually fire-resistant.

green
: This is the venom gee.

Gees move in a linear mannor, and how they move is defined by four
settings you can change by means of this class:

Maximum distance
: This is the complete length of pixels the gee may move over. If it
  reaches one of the ends of this length,
  [is_at_max_distance](#isatmaxdistance) will return `true`.

Flying distance
: This defines the number of pixels the gee moves at one time.

Flying speed
: This is the gee’s velocity when moving.

Waiting time
: This is the number of seconds (plus fractional part) the gee waits
  between two movements. Calling
  [enable_always_flying](#enablealwaysflying) makes the gee ignore
  this setting and never wait between the movements.

Class methods
-------------

### new ########################################################################
    new( moving_direction [, color ] )

Creates a new gee.

#### Parameters

moving_direction
: The direction the gee flys back and forth on. One of `horizontal` or
`vertical`. Other values cause an error.

color (yellow)
: The gee’s color, as explained in the class’ introduction. Other
values cause an error.

Instance methods
----------------

### activate ###################################################################
    activate()

Do one flying movement _now_. The movement will be done according to
the current [max distance](#setmaxdistance), [speed](#setspeed) and
[fly distance](#setflydistance) values.

### disable_always_flying ######################################################
    disable_always_flying()

Prevents the gee from flying always around.

### enable_always_flying #######################################################
    enable_always_flying()

Make the gee ignore the [waiting time](#setwaittime), causing it to
always fly around.

### get_color ##################################################################
    get_color() → a_string

Returns one of the gee colors named in the class’ introduction.

### get_fly_distance ###########################################################
    get_fly_distance() → a_number

Returns the current flying distance. Don’t confuse this with
[get_max_distance](#getmaxdistance).

### get_fly_speed ##############################################################
    get_fly_speed() → a_number

Returns the moving speed of the gee. May include fractions.

### get_max_distance ###########################################################
    get_max_distance() → a_number

Returns the maximum length the gee may move in.

### get_wait_time ##############################################################
    get_wait_time() → a_number

Returns the time between movements of the gee.

### is_always_flying ###########################################################
    is_always_flying() → a_bool

Returns `true` if the gee doesn’t wait between its movements,
otherwise returns `false`. See
[enable_always_flying](#enablealwaysflying).

### is_at_max_distance #########################################################
    is_at_max_distance() → a_bool

If the gee is currently at one of the outer ends of its moving line,
returns `true`, `false` otherwise.

### set_color ##################################################################
    set_color( color )

Change the gee’s type.

#### Parameter
color
: The new color, as explained in the class’ introduction.

### set_fly_distance ###########################################################
    set_fly_distance( distance )

Set the distance the gee may move at one time.

#### Parameter
distance
: The distance to move, in pixels.

### set_fly_speed ##############################################################
    set_fly_speed( speed )

Set the moving speed.

#### Parameter
speed
: The moving speed, may include fractions.

### set_max_distance ###########################################################
    set_max_distance( distance )

Set the maximum distance the gee may move in, i.e. defines the edges
of its moving length.

#### Parameters
distance
: The maximum moving length in pixels.

### set_wait_time ##############################################################
    set_wait_time( time )

Define the time to wait between two movements. 

#### Parameter
time
: The number of seconds to wait. May include fractions.

### stop #######################################################################
    stop()

Stop any movement _now_.
