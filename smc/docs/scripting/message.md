Class: Message
==============

* This is the
{:toc}

This class allows to display simple message boxes to the user. Any
text it receives has to be encoded in UTF-8 (which it is by default if
you set the text from the SMC editor, but may not be the case when
using an external editor for working in the level XML), and any text
it returns is encoded in UTF-8. Long texts cause a scroll bar to
appear at the right side of the displayed message box.

The displayed message box’ dimensions are fixed, and the display
position is calculated from Maryo’s current position, i.e. the message
box will always appear centered above Maryo.

Messages may be displayed multiple times to the user, you don’t have
to create a new instance for repeatingly showing similar messages or
those that build up upon each other (also have a look at
[append()](#append)).

Class methods
-------------

### new ########################################################################
    new( text ) → a_message

Creates a new message. This doesn’t automatically show the message to
the user, you’ve to explicitely call [display()](#display) for that to
happen.

#### Parameter
text
: The text to display. If it doesn’t end in a newline character, one
  is added automatically.

#### Return value
The newly created instance.

Instance methods
----------------

### append #####################################################################
    append( line )

Appends a line of text to the message’s current text.

#### Parameter
line
: A line of text to append. If it doesn’t terminate with a newline
  character, on is automatically added.

### display ####################################################################
    display()

Creates a message box window containing this message’s text and shows
it to the user. While the message box is shown, the rest of the game
is paused and only continues after the user closed the message box by
one of the usual keypresses.

The message box is shown with the position and properties explained in
the class’ introductory text.

### get_text ###################################################################
    get_text() → a_string

The text the message currently contains.

#### Return value

The message’s current text as a string.

### set_text ###################################################################
    set_text( text )

Replace the entire text the message contains.

#### Parameters
text
: The new message text. If it doesn’t end with a newline character,
  one is automatically added.
