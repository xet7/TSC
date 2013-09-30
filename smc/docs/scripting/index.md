SMC Scripting API Documentation
===============================

This is the Secret Maryo Chronicles Scripting API documentation. It
lists all obejcts available from within a level’s script together with
their methods.

SMC scripting is implemented with
[MRuby](https://github.com/mruby/mruby), a minimal implementation of the
[Ruby proggramming language](http://www.ruby-lang.org). Although it’s
not a hard-to-grasp language and intends to be as human-readable as
possible, it tries to not sacrifice principles of powerful
object-oriented programming. So, while you may understand and even
write scripts without a thorough knowledge of the Ruby language, the
key to mastering SMC’s scripting mechanism is an at least rudimental
understand of Ruby.

For those that are already familiar with the canonical Ruby
interpreter ("MRI") or one of the other great Ruby implementations
such as Rubinius or JRuby, let me point out that the "m" in "mruby"
really stands for "minimal". Don’t expect to get a fully-featured Ruby
implementation, especially regarding the standard library or RubyGems,
which are simply missing. However, the language core itself should
mostly be the way you expect it.

Kinds of MRuby objects
----------------------

There are three kinds of objects you may interact while scripting SMC:

Classes
: These are the main component of the scripting API. You can create
  instances of these classes by calling their `::new` method, which
  will give you in most cases an ordinary MRuby object that is wrapped
  around some C++ pointer. Their _instance methods_ give you the
  possibility to interact with the underlying C++ pointer, both
  modifying and inspecting the object it points to. Note that classes
  themselves are objects, but with no internal C++ pointer as none is
  needed for them.

C++ data
: As noted before, instances of (most) of SMC’s MRuby classes wrap
some kind of C++ pointer. In most cases you will not have to worry
about it, but depending on the exact usecase rethinking this fact may
come in handy. You can’t access these pointers directly (Ruby is a
pointerless language), but the object’s methods will do this
internally for you.

Singletons
: There’s a little number of special objects called _singletons_.
  You can treat them mostly like normal MRuby objects, but the
  class these singletons belong to is not instanciatable from the
  MRuby side, e.g. the `Audio` singleton is the one and only instance
  of the [AudioClass](audioclass.html) class.

Unique Identifiers (UIDs)
-------------------------

Each sprite created via the regular SMC editor (a so-called _internal_
sprite) is assigned an identifier that is unique for the whole of the
current level, hence it is called _unique identifier_, or short
_UID_. You can determine an internal sprite’s UID by loading your
level into the SMC editor and hover the cursor over the object whose
UID you want to know; there the UID is displayed next to the
coordinates of the object. These UIDs are guaranteed to stay the same
between multiple level loads and even level editing (however, deleting
an object in the editor will release its UID and make it available to
other sprites). SMC maintains a global MRuby object called `UIDS` that
references a table which maps all known UIDs to specific instances of
class [Sprite](sprite.html) or one of its subclasses. This makes it
easy to refer to a specific sprite and interact with it. For example,
if you wanted to move a block with UID 38 away (e.g. for unblocking a
path):

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ruby
# Note that (-100|100) is outside the visible area,
# therefore it looks as if the block "disappeared".
UIDS[38].warp(-100, 100)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Sprites created via the scripting interface (so-called _external_
sprites) do not have an UID assigned to them automatically, therefore
they won’t show up in the global `UIDS` table. The
[Sprite](sprite.html) class’ [new()](sprite.html#new) method (and the
`new()` methods of its subclasses) however support an optional last
`uid` parameter that allows you to specify a UID for external
sprites. Note that specifying an already used UID will cause an error,
therefore you probably want to either use UIDs surely not used
(something above 10000, as levels with that many elements are
extraordinarily rare) or query the length of the current UID table
(which ideally has no gaps, but that cannot be guaranteed) and adding
to it dynamically:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ruby
mysprite = Sprite.new("path/to/pic", UIDS.count + 1)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ruby
UIDS[38].on_touch do |other|
  Player.kill! if other.player?
end
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Furthermore, events are inherited. This allows you for example to
register for the Touch event of a Furball, which somewhere up the
inheritance chain is a Sprite. Therefore, if you don’t find the event
you’re looking for in your object’s class’ documentation, try the
superclass’ documentation.

Organisation of the documentation
---------------------------------

The documentation is created by a helper (Ruby) program called
`gen_docs.rb` in the SMC source tree. This program parses the C++
files implementing the scripting interface and extract specifcally
marked documentation comments, transforms them into HTML and writes
them out to the `smc/docs/html` directory. Most likely you’re looking
at the `index.html` file in this directory now (if not, you’re reading
the Markdown sources directly).

The documentation is divided into the classes which you can access
from the MRuby side. Their descriptions always start with a general
overview of what the class/the singleton is supposed to be used for,
followed by the list of events and then a list of methods:

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
that actually do _not_ return anything interesting), and that this
something is `a_bool`, i.e. either `true` or `false`. You don’t have
to care for the return value if you don’t want to.

The method description continues with a detailed look on the parameter
list, describing each parameter and its default value (as already
explained above).

After this, the documentation may include a short paragraph about the
exact nature of the return value (if it’s not obvious from the name or
the call sequence).

And finally, if you’re lucky the method’s documentation may contain
some usage examples of the method. But again, this may not be the case
for all methods.

List of classes, modules, and singletons
----------------------------------------

This is an alphabetical list of all classes, modules, and singletons exposed to
the MRuby scripting API:

* [AnimatedSprite](animatedsprite.html)
* Audio → [AudioClass](audioclass.html)
* [AudioClass](audioclass.html)
* [BonusBox](bonusbox.html)
* [Box](box.html)
* [Eato](eato.html)
* [Enemy](enemy.html)
* [Eventable](eventable.html)
* [Fireplant](fireplant.html)
* [Flyon](flyon.html)
* [Furball](furball.html)
* [Gee](gee.html)
* Input → [InputClass](inputclass.html)
* [InputClass](inputclass.html)
* [Krush](krush.html)
* Level → [LevelClass](levelclass.html)
* [LevelClass](levelclass.html)
* [LevelPlayer](levelplayer.html)
* [Message](message.html)
* [Moon](moon.html)
* [MovingSprite](movingsprite.html)
* [Mushroom](mushroom.html)
* [ParticleEmitter](particleemitter.html)
* [Path](path.html)
* [Path::Segment](path_segment.html)
* Player → [LevelPlayer](levelplayer.html)
* [Powerup](powerup.html)
* [Rokko](rokko.html)
* [SMC](smc.html)
* [Spika](spika.html)
* [Spikeball](spikeball.html)
* [SpinBox](spinbox.html)
* [Sprite](sprite.html)
* [Star](star.html)
* [StaticEnemy](staticenemy.html)
* [TextBox](textbox.html)
* [Thromp](thromp.html)
* [Timer](timer.html)
* [Turtle](turtle.html)
* [TurtleBoss](turtleboss.html)
* [UIDS](uids.html)

License
-------

The API documentation is licensed under the same terms as SMC, unless
noted otherwise.
