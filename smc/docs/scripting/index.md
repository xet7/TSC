SMC Scripting API Documentation
===============================

This is the Secret Maryo Chronicles Scripting API documentation. It
lists all obejcts available from within a level’s script together with
their methods.

SMC scripting is implemented with
[MRuby](https://github.com/mruby/mruby), a minimal implementation of the
[Ruby programming language](http://www.ruby-lang.org). Although it’s
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
implementation, especially regarding the standard library or _RubyGems_,
which are simply missing. However, the language core itself should
mostly be the way you expect it.

Interaction with C++
--------------------

SMC is written in C++. As such, all method calls that actually _do_
anything in the gameplay must be translated to C++ function calls. For
this to happen, each SMC-related mruby object is wrapped around a C++
pointer that points to the actual underlying C++ object. Whenever you
call a method on the mruby object, that method unwraps the C++
pointer, translates the mruby arguments you hand to the method to
types usable for C++ and then calls the C++ object’s corresponding
function. On returning, the same process happens in reverse order.

While this knowledge may not seem important to you, it may help you
fixing obscure problems related to the fact the mruby objects are
independent from the underlying C++ pointers.

Object types
------------

There are two ways to interact with SMC objects. The first way is to
get hold of an object already existing in the level, for example an
enemy or a block. The other way is to actively _create_ an object
yourself, by calling the corresponding object’s `::new` method.

Once you have an mruby object you can interact with, there’s no
difference anymore between mruby objects build up around existing C++
ones and mruby objects that were created together with a C++ object.

### Retrieval of existing objects ###

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
class [Sprite](sprite.html) or one of its subclasses.

In order to actually create an mruby object for an existing C++ object
in the level, your way goes through that `UIDS` object. It provides
you with a method `::[]` that determines what C++ pointer belongs to
the UID you pass it, wraps an mruby object around the pointer and
finally returns that mruby object to you. The returned mruby object is
then cached, so if you query `UIDS` again with the same UID, you will
get the _exact same object_ back as for the first time you queried.

For example, if you wanted to unblock a path by moving a block with
UID 38 away:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ruby
# Note that (-100|100) is outside the visible area,
# therefore it looks as if the block "disappeared".
block = UIDS[38]
block.warp(-100, 100)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Creation of new objects ###

As mentioned above, there’s a second way to interact with the
level. Instead of passively retrieving existing objects and
interacting with them, you can actually _create_ new things and place
them in the level. To achieve this, call the corresponding class’
`::new` method, which will generate a new C++ pointer and a new mruby
object for that pointer, with all its attributes set to SMC’s default
values for that type.

As an example, here’s how you would proceed for spawning a furball:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ruby
furball = Furball.new
furball.start_direction = :right
furball.start_at(300, -300)
furball.show
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The first line as explained generates a new furball. We then make that
furball look into the right, while (line 3) it initially appears at
position `(300|-300)`. From the moment you call the `#show` method on,
your settings will take effect and the new furball takes part in the
gameplay.

Hint: Don’t forget to call `#show` on your generated objects,
otherwise they won’t show up at all.

These generated or _external_ sprites are automatically marked as
_generated_ objects and hence are **not** saved when the user creates
a new savegame! If you want to preserve your generated objects through
saving/loading a savegame, you have to register for specific events on
the level; see the documentation on [Level](levelclass.html) for more
information on this.

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
  other.kill! if other.player?
end
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Furthermore, events are inherited. This allows you for example to
register for the Touch event of a Furball, which somewhere up the
inheritance chain is a Sprite (where the Touch event is initially
defiend). Therefore, if you don’t find the event you’re looking for in
your object’s class’ documentation, try the superclass’ documentation.

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
   When referencing class methods in the documentation, we will
   always use a double-colon like this: `Furbal::new` means the
   `new()` method on the `Furball` class.
2. The instance methods. These are methods you can call on instances
   of the respective class. When referencing instance methods in
   the documentation, we will alawys use a hash symbol like
   this: `MovingSprite#warp` refers to the `warp()` method of
   objects of type `MovingSprite`.

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
the MRuby scripting API, grouped by topic:

| Name & link                           | Notes                                       |
|---------------------------------------|---------------------------------------------|
| [SMC](smc.html)                       | Game-related stuff.                         |
| [UIDS](uids.html)                     | Global UID table                            |
|---------------------------------------|---------------------------------------------|
| ***Sprites***                         | ***Sprites***                               |
|---------------------------------------|---------------------------------------------|
| [AnimatedSprite](animatedsprite.html) |                                             |
| [MovingSprite](movingsprite.html)     |                                             |
| [Sprite](sprite.html)                 | Base class for nearly everything            |
|---------------------------------------|---------------------------------------------|
| ***Enemies***                         | ***Enemies***                               |
|---------------------------------------|---------------------------------------------|
| [Enemy](enemy.html)                   | Base class for all enemies                  |
| [Eato](eato.html)                     |                                             |
| [Flyon](flyon.html)                   |                                             |
| [Furball](furball.html)               |                                             |
| [Gee](gee.html)                       |                                             |
| [Krush](krush.html)                   |                                             |
| [Rokko](rokko.html)                   |                                             |
| [Spika](spika.html)                   |                                             |
| [Spikeball](spikeball.html)           |                                             |
| [Thromp](thromp.html)                 |                                             |
| [Turtle](turtle.html)                 |                                             |
| [TurtleBoss](turtleboss.html)         |                                             |
| [StaticEnemy](staticenemy.html)       |                                             |
|---------------------------------------|---------------------------------------------|
| ***Boxes***                           | ***Boxes***                                 |
|---------------------------------------|---------------------------------------------|
| [Box](box.html)                       | Base class for all boxes                    |
| [BonusBox](bonusbox.html)             |                                             |
| [Spinbox](spinbox.html)               |                                             |
| [TextBox](textbox.html)               |                                             |
|---------------------------------------|---------------------------------------------|
| ***Powerups***                        | ***Powerups***                              |
|---------------------------------------|---------------------------------------------|
| [Powerup](powerup.html)               | Base class for all powerups                 |
| [Fireplant](fireplant.html)           |                                             |
| [Moon](moon.html)                     |                                             |
| [Mushroom](mushroom.html)             |                                             |
| [Star](star.html)                     |                                             |
|---------------------------------------|---------------------------------------------|
| ***Level stuff***                     | ***Level stuff***                           |
|---------------------------------------|---------------------------------------------|
| Level → LevelClass                    | The level itself; singleton                 |
| [LevelClass](levelclass.html)         |                                             |
| [LevelExit](levelexit.html)           |                                             |
| [LevelPlayer](levelplayer.html)       |                                             |
| Player → LevelPlayer                  | Maryo; singleton                            |
|---------------------------------------|---------------------------------------------|
| ***Miscellaneous***                   | ***Miscellaneous***                         |
|---------------------------------------|---------------------------------------------|
| Audio → AudioClass                    | Singleton                                   |
| [AudioClass](audioclass.html)         |                                             |
| [Eventable](eventable.html)           |                                             |
| Input → InputClass                    | Singleton                                   |
| [InputClass](inputclass.html)         |                                             |
| [Path](path.html)                     |                                             |
| [Path::Segment](path_segment.html)    |                                             |
| [Timer](timer.html)                   |                                             |

License
-------

The API documentation is licensed under the same terms as SMC, unless
noted otherwise.
