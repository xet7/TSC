Class: Timer
==============

* This is the
{:toc}

_Timers_ are an easy and efficient way to execute Lua code based on a
time span. They can either be periodic, meaning that they will
continue to be active for an unspecified amount of time, or
non-periodic aka one-shot, meaning that they will only be active for a
definite timespan. For both types of timers, you have to call the
[start()](#start) method to activate them, and it’s possible to
interrupt a timer by means of its [stop()](#stop) method (however,
interrupting a one-shot timer doesn’t necessarily make sense).

Both types of timers employ a _callback_ concept similar to how events
work. When creating a timer with [new()](#new) or one of the
convenience methods [after()](#after) and [every()](#every), you pass
a _callback function_ to the method that will be copied to an internal
location and when the timer fires, this local copy is called (so
overwriting the function in the meantime will have no effect).

A _periodic_ timer enters an infine loop when [start()](#start) is
called on it. It will then wait the inverval specified when creating
the timer and execute the callback. That process is repeated until you
either call [stop()](#stop) or end the level.

A _non-periodic_ timer doesn’t loop. When [start()](#start) is called,
it waits the amount of time it is configured for (just like a periodic
timer does) and then executes the callback function. However, the
timer will not continue to do anything beyond this. No looping is
done, nor any cleanup.

Timers of any type do *not* run in parallel. Although the actual
marking of callbacks for run indeed *is* asynchronous, the callback is
executed while evaluating the game’s regular mainloop (a consequence
of this is that your callback won’t be called with 100% accuracy
regarding the timespan, it will be cropped to the next
frame). Therefore it is recommended to not put very time-consuming
actions into a timer’s callback function as it will slow down the
entire game. For example, you do _not_ want to calculate π inside your
timer’s callback function. Moving objects around on the other hand
should be OK.

Note a particularity with objects of this class: Even when a timer
goes out of scope, it doesn’t cease to exist. So, if you create a
periodic timer like this:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
function callback()
  print("callback")
end

function create_timer()
  local timer = Timer:every(1000, callback)
  timer:start()
end

create_timer()
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
{:lang="lua"}

You might expect the periodic timer to stop working when the `timer`
variable goes out of scope and the Lua userdata it references gets
garbage-collected. This his however not the case; to prevent you from
having to create a massive amount of global objects for your timers, a
C++ timer, once started, will continue to work even after its Lua
counterpart has gone out of scope. The bottomside of this is that you
can’t influence the timer anymore after it has been garbage-collected,
there is no way to get a reference to it again. The only thing you can
do to force the C++ timer to stop is to end the level, which is
probably not what you want :-).

Having that said, I want to enourage you to be brave and let your
timers go out of scope. You do not have to call [stop()](#stop)
manually when ending the level, this is done automatically for you. So
no reason to clutter the global scope with timers.

Last but not least you shouldn’t use [new()](#new) directly. Use the
[after()](#after) and [every()](#every) class methods instead, as they
make your intention more clear and are more readable than a `true` or
`false` passed to [new()](#new).

Class methods
-------------

### after ######################################################################
    after( interval, callback ) → a_timer

Shortcut for calling [new()](#new) with `is_periodic = false`. See
[new()](#new) for explanations on the parameters.

#### Return value

The newly created instance.

### every ######################################################################
    every( interval, callback ) → a_timer

Shortcut for calling [new()](#new) with `is_periodic = true`. See
[new()](#new) for explanations on the parameters.

#### Return value

The newly created instance.

### new  ########################################################################
    new( interval, callback [, is_periodic ] ) → a_timer

Creates a new `Timer` instance, either periodic or non-periodic
depending on the last parameter’s value.

#### Parameters

interval
: The timespan to configure the timer for, in milliseconds. With a
  periodic timer, this is the waiting time between calls to your
  callback function, with a non-periodic timer this is the time to
  wait before the one and only call to your callback function.

callback
: A Lua function to run when the timer fires. Does not have to be a
  named function, anonymous ones are OK as well. Note this is copied,
  so altering the function afterwards won’t have any effect.

is_periodic (false)
: If this is a truth value, create a periodic (repeating) timer
  instead of a non-repeating (one-shot) timer.

#### Return value

The newly created instance.

Instance methods
----------------

### get_interval ###############################################################
    get_interval() → a_number

Returns the time interval for this timer, in milliseconds.

### is_active ##################################################################
    is_active() → a_boolean

Returns `true` if [start()](#start) has been called without a
consecutive call to [stop()](#stop), i.e. returns `true` if the timer
is running. This also returns `true` for one-shot timers that have
finished if [stop()](#stop) has not been called on them.

### start ######################################################################
    start()

Set the timer active.

### stop #######################################################################
    stop()

Stop the timer as soon as possible. Note this usually doesn’t mean the
timer is stopped immediately and it may be that the timer’s callback
function is registered to run yet again.

This method **blocks** until the timer has stopped. Think twice before
using this method, because this means the game’s main loop is completely
paused until this method returns. The user won’t be able to do any
interaction with the game in this time.
