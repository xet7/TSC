Image settings files format
===========================

Images have associated settings files defining meta-data used by the game.
They contain a list of key-value pairs on different lines. Each line is a
space-separated list of terms where the first term is a word identifying the
key and the following terms present an array of values.

Here is an example of settings text, from `pixmaps/enemy/eato/brown/1.settings`:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
width 36
height 36
col_rect 4 14 28 22
author Helios
name brown
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The attributes are as follows:

# `name *N*`

Name of the image as presented in the editor. Spaces are not allowed, and
underscores (`_`) should be used in their place.

* `base *B* *I*`

*B* refers to an image file that these settings modify. By default, the base
image is assumed to be named the same as the settings file but with the `.png`
extension in place of `.settings`. If *I* is non-zero, then the `.settings`
file that *does* have the same name as the image, if one exists, is inherited
as the base for the current settings file (all attributes default to how they
are set in the inherited file). Otherwise, if *I* is zero, no such inheritance
is attempted.

Example:

~~~
base beanstalk_2.png 1
~~~

# `int_x *X*`, `int_Y *Y*`

Define the "internal drawing offset" X and Y positions. TODO : explain.

# `width *W*`, `height *H*`

Define the width and height of the image to be *W* and *H*, respectively.

# `rotation *X* *Y* *Z*`

Define the X, Y, and Z rotation of the image.

# `col_rect *X* *Y* *W* *H*`

The collision algorithm used by the game is called a "bounding-box collision
detection algorithm", where all sprites in the game are given a "box" (i.e.
rectangular constraint) which is checked for overlap with other boxes to
detect collisions. col_rect will specify the X and Y (from left and from top)
positions of the rectangle relative to the image itself while width and height
define the width and height of the rectangle.

# `type *T*`

Define default massive type, which can be one of:

* `passive`: background element with no effect on player or enemies
* `font_passive`: foreground element with no effect on player or enemies
* `massive`: physical object on same layer as player and enemies
* `halfmassive`: like massive but pressing down lets you jump down
* `climbable`: the object is a background element that may be climbed

# `ground_type *G*`

Define the ground type of the image, which can be one of `normal`, `earth`,
`ice`, `sand`, `stone`, or `plastic`.

TODO  : is this used for categorization?

# `editor_tags *E*`

Define semicolon-separated list of editor tags. TODO : list editor tags.

# `mipmap *M*`

*M* is non-zero = mipmapped
*M* is zero = not mipmapped

TODO : explain what this means

# `author *A*`, `license *L*`

Meta-data determining the author and license of the image asset.

# `obsolete *O*`

If *O* is non-zero, then the image is obsolete. TODO : look up what exactly
this means to the game.
