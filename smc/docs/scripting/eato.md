Class: Eato
===========
Parent: [Enemy](enemy.html)
{: .superclass}

* This is the
{:toc}

![Eato](graphics/eato.png){:.enemyimg} _Eatoes_ are mostly
fire-resistant plants that usually hang around everywhere you want to
get through. They usually occur in large groups to block the path in
front of Maryo.

Eatoes currently exist in two flavours, namely the _brown_ eato (shown
above) and the _green_ eato. You can add your own eato types by
creating a new directory under `pixmaps/enemy/eato` that is named
after the colour you want to introduce. Look at the existing colours to
get an idea of how to organise the files in that directory.

Class methods
-------------

### new ########################################################################
    new( direction [, image_directory [, uid ] ] ) → an_eato

Creates a new eato.

#### Parameters
direction
: The direction to look into. One of the following strings:

  * `up_left`
  * `up_right`
  * `left_up`
  * `left_down`
  * `right_up`
  * `right_down`
  * `down_left`
  * `down_right`

  Note I don’t get the difference myself. If you try, you will see
  that the second component of the string is silently ignored, i.e. an
  `up_left` eato always looks up. Nothing with left. Even worse, the
  strings starting with `left` and `right` seem to be swapped, so a
  `left_down` eato will actually look to the right.

image_directory (`"enemy/eato/brown/"`)
: Where to look up the graphcis for this eato. As described in the
  class’ introduction, eato graphics usually reside under
  `enemy/eato/`, so if you want to add your own eato colours you’re
  encouraged to put them there. This parameter however is relative to
  the `pixmaps/` directory directly as this is what SMC itself
  expects. Note the argument should terminate with a trailing slash
  (to indicate it’s a directory).

uid (nil)
: UID to assign to this sprite. UIDs already in use cause an error.

#### Return value

The newly created eato.

Instance methods
----------------

### get_image_dir ##############################################################
    get_image_dir() → a_string

Returns the path to the directory containing this eato’s image files,
relative to the `pixmaps/` directory. The returned strings terminates
with a trailing slash `/` as it’s a directory path.

### set_image_dir ##############################################################
    set_image_dir( path )

Sets the path to the directory containing this eato’s image files.

#### Parameter
path
: Path to the directory, relative to the `pixmaps/` directory. This
  argument should terminate with a final slash `/` as it’s a directory
  path.
