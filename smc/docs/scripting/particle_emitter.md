Class: ParticleEmitter
======================

* This is the
{:toc}

The _ParticleEmitter_ is one of the most complex classes in the
API. If you’re not familiar with using regular particle emitters from
the normal SMC editor, you probably want to go there first and
experiment with them, because everything you need to adjust there
needs to be adjusted for dynamically created particle emitters as
well--with the difference that in the editor you have a nice UI
assisting you, whereelse for the dynamically created particle emitters
you have to _know_ what to set and what not to set. Particle emitters
can be quite hairy beasts, so I recommend you to often load your level
and test what your particle emitter will look like, e.g. by
registering for Maryo’s _Jump_ event.

Particle emitters can be used in two ways: The usual way, which means
periodically issueing a defined number of particles at a time. Such a
particle emitter can be created by setting the [emitter’s time to
live](#setemittertimetolive) on that particle emitter. If you want more
finergrained control about what is going on, you can ignore that
setting and call the [emit()](#emit) method directly. Each call to that
method will cause the particle emitter to exactly once emit particles
according to its configuration.

A good number of setter methods accepts a parameter named `rand` that
allows to randomise an otherwise statically set value. For instance,
the [set_time_to_live()](#settimetolive) method allows you to define
the lifespan of particles emitted at a time. By default, the `rand`
modification is 0, i.e. the value is taken as-is. So, if you call

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
e:set_time_to_live(10)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
{:lang="lua"}

(where `e` is a `ParticleEmitter` instance), all particles will have
the same lifespan of 10 seconds. However, if you pass a second
parameter, as in

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
e:set_time_to_live(10, 2)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
{:lang="lua"}

then this argument will be taken as a random modifier of the
operation. This means that in the above example all particles will
have a lifespan of

    8 <= l <= 12

, where `l` is a single particle’s lifespan. This randomisation is
done for all particles, allowing you to create more vidid particle
emitters that create particles that don’t all behave the same way.

Note that, in contrast to all other objects in SMC, it is possible to
set a particle emitter’s [Z coordinate](#setz), making it possible to
appear in front of Maryo or other sprites.

Also note that `ParticleEmitter` is not a subclass of `Sprite` (the
particle emitter doesn’t show up on the screen itself, just its
emitted particles) and the methods defined there don’t apply here
therefore.

Class methods
-------------

### new ########################################################################
    new(x, y [, width [, height ] ] ) → a_particle_emitter

Creates a new particle emitter. It won’t emit particles by default,
you first have to adjust the emitter with the various set_* methods,
and when you’ve done this you can either call emit() which will
gives you absolute control over each emitted particle, or use
set_emitter_time_to_live() to make the emitter emit particles
automatically.

#### Parameters
x
: The X coordinate of the emitter.

y
: The Y coordinate of the emitter.

width (nil)
: The horizontal area in in pixels which particles will be created.

height (nil)
: The vertical area in pixels in which particles will be created.

#### Return value

The newly created instance.

Instance methods
----------------

### emit #######################################################################
    emit()

Emit a single particle (or multiple ones if the [quota](#setquota) is
set accordingly). Usually you want to use
[set_emitter_time_to_live()](#setemittertimetolive) to make the
emitter act automatically, but this method allows to retain full
control about the particle emitter.

### get_const_rotation_x #######################################################
    get_const_rotation_x() → rotation, rand

Returns the constant X rotation.

#### Return value

The base `rotation` and the modifier `rand` (see the class’ docs)

### get_const_rotation_y #######################################################
    get_const_rotation_y() → rotation, rand

Returns the constant Y rotation.

#### Return value

The base `rotation` and the modifier `rand` (see the class’ docs)

### get_const_rotation_z #######################################################
    get_const_rotation_z() → rotation, rand

Returns the constant Z rotation.

#### Return value

The base `rotation` and the modifier `rand` (see the class’ docs)

### get_emitter_time_to_live ###################################################
    get_emitter_time_to_live() → a_number

Returns the number of seconds the whole emitter may live. Don’t
confuse this with [get_time_to_live()](#gettimetolive), which is for
the particles.

-1 means to live forever.

### get_gravity_x ##############################################################
    get_gravity_x() → gravity, rand

Returns the horizontal gravity.

#### Return value

The base `gravity` and the `rand`om modifier (see the class’ docs).

### get_gravity_y ##############################################################
    get_gravity_y() → gravity, rand

Returns the vertical gravity.

#### Return value

The base `gravity` and the `rand`om modifier (see the class’ docs).

### get_image_filename #########################################################
    get_image_filename() → a_string

Returns the path to the currently emitted particle’s
image file, relative to the `pixmaps/` directory.

### get_quota ##################################################################
    get_quota() → a_number

Returns the amount of particles emitted at one time.

### get_scale ##################################################################
    get_scale() → scale, rand

Returns the scaling information about created particles.

#### Return value

`scale` is the base scale (1 meaning original image size, smaller
values meaning shrinking and larger values meaning extending). `rand`
is the random scale modifier (see the class’ docs).


### get_speed ##################################################################
    get_speed() → speed, rand

Return the current emitting speed.

#### Return value

`speed` is the base emitting interval in seconds, `rand` the random
modifier (see the class’ docs).

### get_time_to_live ###########################################################
    get_time_to_live() → ttl, rand

Returns the TTL of this particle emitter’s particles. Don’t confuse
this with [get_emitter_time_to_live()](#getemittertimetolive) which is
for the whole emitter.

#### Return value

`ttl` is the base time to live, `rand` defines the random plusminus
area for the TTL.

### set_const_rotation_x #######################################################
    set_const_rotation_x( rotation [, rand ] )

Set the constant X rotation.

#### Parameters
rotation
: The base rotation.

rand (0)
: The modifier (see the class’ docs).

### set_const_rotation_y #######################################################
    set_const_rotation_y( rotation [, rand ] )

Set the constant Y rotation.

#### Parameters
rotation
: The base rotation.

rand (0)
: The modifier (see the class’ docs).

### set_const_rotation_z #######################################################
    set_const_rotation_z( rotation [, rand ] )

Set the constant Z rotation.

#### Parameters
rotation
: The base rotation.

rand (0)
: The modifier (see the class’ docs).

### set_emitter_time_to_live ###################################################
    set_emitter_time_to_live( time )

Sets the time the whole emitter may live (don’t confuse this with
[set_time_to_live()](#settimetolive), which is for the particles). -1
means to live forever.

#### Parameter
time
: The lifespan in seconds.

### set_gravity_x ##############################################################
    set_gravity_x( gravity [, rand ] )

Sets the horizontal gravity for this particle emitter.

#### Parameters
gravity
: The base gravity.

rand (0)
: The random modifier. See the class’ docs.

### set_gravity_y ##############################################################
    set_gravity_y( gravity [, rand ] )

Sets the vertical gravity for this particle emitter.

#### Parameters
gravity
: The base gravity.

rand (0)
: The random modifier. See the class’ docs.

### set_image_filename #########################################################
    set_image_filename( path )

Set the path of the image to emit. The `path` is relative to the
`pixmaps/` directory.

### set_quota ##################################################################
    set_quota( quota )

Sets the amount of particles emitted at one time.

### set_scale ##################################################################
    set_scale( scale [, rand ] )

Set the scale of the emitted particles.

#### Parameters
scale
: The base scaling factor, 1 meaning original image size, 0.5 half, 2
  double. Other values behave accordingly.

rand (0)
: Random scale modifier. See the class’ docs.

### set_speed ##################################################################
    set_speed( speed [, rand] )

Set the emitting speed.

#### Parameters
speed
: The emitting interval, in seconds.

rand (0)
: The random modifier (see the class’ docs).

### set_time_to_live ###########################################################
    set_time_to_live( time [, rand ] )

Set the time the particles may live. Don’t confuse this with
[set_emitter_time_to_live()](#setemittertimetolive), which is for the
whole emitter.

#### Parameters
time
: The base TTL.

rand (0)
: Random plusminus modifier for the TTL.

### set_z ######################################################################
    set_z( val )

Set the Z coordinate.
