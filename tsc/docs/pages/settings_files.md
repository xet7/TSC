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

* width: defines the width of the image in pixels
* height: defines the height of the image in pixels
* col_rect: defines the collision rectangle as X, Y, Width, and Height
* author: defines the author name (for reference); use underscores for spaces
* name: ??? (TODO : figure out what this is used for...)

The collision algorithm used by the game is called a "bounding-box collision
detection algorithm", where all sprites in the game are given a "box" (i.e.
rectangular constraint) which is checked for overlap with other boxes to
detect collisions. col_rect will specify the X and Y (from left and from top)
positions of the rectangle relative to the image itself while width and height
define the width and height of the rectangle.
