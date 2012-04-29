SMC Scripting API Documentation
===============================

This is the Secret Maryo Chronicles Scripting API documentation. It
lists all obejcts available from within a level’s script together with
their methods.

SMC scripting is implemented with [Lua](http://lua.org) and currently
uses version 5.2 of the Lua implementation. Before you start adding
scripts to your levels, you probably want to learn Lua; it’s not a
difficult language, so if you’re already familiar with programming in
general, it should be doable in about one day. The canonical resource
for the Lua language is
[the Lua manual](http://www.lua.org/manual/5.2/manual.html).

Kinds of Lua objects
--------------------

There are three kinds of objects you may interact while scripting SMC:

Classes
: These are the main component of the scripting API. You can create
  instances of these classes by calling their `new()` method, which
  will give you in most cases a special object called a Lua
  _userdata_. You can call the _instance methods_ defined for a given
  class on these objects and they will react accordingly. Classes
  themselves are no userdata objects, just ordinary Lua tables with a
  key called `new` that maps to a special C++ function that creates
  the userdata instances for you.

Userdata
: As noted before, (full) userdata objects are mostly instances of the
  classes SMC exposes to the Lua API. Internally they wrap a pointer
  to an instance of one of SMC’s C++ classes, and through the methods
  provided by the userdata objects you can interact with what the
  internal C++ pointer points to.

Singletons
: There’s a little number of special objects called _singletons_.
  You can treat them mostly like normal userdata objects, but the
  class these singletons belong to is not instanciatable from the
  Lua side, e.g. the `Audio` singleton is the one and only instance
  of the [AudioClass](audio_class.html) class.

About method calling
--------------------

Whenever you call a method on _any_ object from Lua, you have to use
the colon syntax which tells Lua to implicitely pass the receiver as
the first argument to the underlying function. In order to avoid
confusion about when to use the colon syntax and when not to use it,
this rule includes singletons as well, although it would technically
be possible to call a singleton’s method without a receiver (because
they internally use a special SMC global pointer instead of whatever
is stored inside the userdata). So, this is an error:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Audio.play_sound("waterdrop_1.ogg")
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
{:lang="lua"}

_Always_ use the colon syntax, regardless of what you call methods
on.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Audio:play_sound("waterdrop_1.ogg")
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
{:lang="lua"}

Events
------

The scripting API uses an event-driven approach, i.e. whenever you’re
interested in intercepting a given action, you’re going to register an
_event handler_ for a specific _event_. Each event is a bit different,
especially regarding the information they pass in form of arguments to
their event handlers, but the process of registering for an event is
always the same.

1. You want to listen for a specific event, e.g. the _Touch_ event.
2. You write an event handler accepting the parameters the event will
   hand to you (e.g. the other sprite in case of the Touch event).
3. You register the handler by calling the object’s `on_<eventname>`
   method, e.g. `on_touch` for the Touch event, and passing it the
   function you wrote.

The registering methods are always named `on_<downcased_eventname>`,
so once you know which event to listen for, registering it is quite
simple. For example, if you want to kill the player when he
collides with the object with UID 38, you’d do:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
function myhandler(other)
  Player:kill()
end

Sprite[38]:on_touch(myhandler)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
{:lang="lua"}

Furthermore, events are inherited. This allows you for example to
register for the Touch event of a Furball, which somewhere up the
inheritance chain is a Sprite. Therefore, if you don’t find the event
you’re looking for in your object’s class’ documentation, try the
superclass’ documentation.

Organisation of the documentation
---------------------------------

Each file inside the directory **smc/docs/scripting** of the
Secret Maryo Chronicles sourcecode tree describes one class or
singleton. Descriptions always start with a general description of
what the class/the singleton is supposed to be used for, followed by
the list of events and then a list of methods:

1. The class methods. These are methods you can directly call on the
   class object, without having to create an instance of that class.
2. The instance methods. These are methods you can call on instances
   of the respective class.

Each method is introduced using its name, followed by a list of one or
more possible call sequences. A call sequence may look like this:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
play_sound( filename [, volume [, loops [, resid ] ] ] ) → a_bool
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This can serve as a quick reference on how to use the method. It tells
you that the `filename` parameter is required (because it isn’t
included in any brackets), but the other ones, like `volume` or
`resid`, are optional and don’t have to be passed. Fitting default
values will be assumed (the exact default values can be found in the
_Parameters_ section of the respective method, where it is included in
parantheses after the parameter’s name). Finally, the → indicates that
the methods returns something (there exist quite a number of methods
that actually do _not_ return anything), and that this something is
`a_bool`, i.e. either `true` or `false`. You don’t have to care for
the return value if you don’t want to.

The method description continues with a detailed look on the parameter
list, describing each parameter and its default value (as already
explained above).

After this, the documentation may include a short paragraph about the
exact nature of the return value (if it’s not obvious from the name or
the call sequence).

And finally, if you’re lucky the method’s documentation may contain
some usage examples of the method. But again, this may not be the case
for all methods.

List of classes and singletons
------------------------------

This is an alphabetical list of all classes and singletons exposed to
the Lua scripting API:

* [AnimatedSprite](animated_sprite.html)
* Audio → [AudioClass](audio_class.html)
* [AudioClass](audio_class.html)
* [Eato](eato.html)
* [Enemy](enemy.html)
* [Flyon](flyon.html)
* [Furball](furball.html)
* [Gee](gee.html)
* [LevelPlayer](level_player.html)
* [Message](message.html)
* [MovingSprite](moving_sprite.html)
* [ParticleEmitter](particle_emitter.html)
* Player → [LevelPlayer](level_player.html)
* [Sprite](sprite.html)

License
-------

The API documentation is licensed under the same terms as SMC, unless
noted otherwise.
