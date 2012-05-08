Class: InputClass
=================

* This is the
{:toc}

The sole instance of this class is the `Input` singleton which allows
you to register for events regarding direct user interaction. Other
than that, this class is pretty useless.

Events
------

Key_Down
: Triggered when the player presses one of the SMC-relevant keys,
  e.g. the action or jump key. The event handler gets passed the name
  of the key in question as a string, i.e. "action", "jump",
  etc. Instead of listing all possible keys here I encourage you to
  register for the event and print out the handler’s argument in the
  console.

  The event is also fired for joystick input.
