Secret Maryo Chronicles
=======================

What is it?
-----------

From the official website at http://www.secretmaryo.org:

> Secret Maryo Chronicles is an Open Source two-dimensional platform
> game with a design similar to classic computer games. SMC has
> computer support to a great degree by using an accelerated [Open
> Graphics Library](http://opengl.org) for the best possible graphic
> design and stock performance.

It features a wealth of levels, powerups, great music and much more —
see http://www.secretmaryo.org/index.php?page=about for a more
comprehensive list. And for those still not getting enough, it
provides a great in-game level editor.

This repository contains my (@Quintus) fork of SMC, because upstream
development at https://github.com/FluXy/SMC seems to have more or less
stalled and I’ve been missing some features, most notably the often
requested scripting functionality for SMC. The final goal hover still
is to merge my changes with upstream.

Wait, scripting?
----------------

Yes. This repository’s main purpose is to implement scripting with
[mruby](https://github.com/mruby/mruby) into SMC. While it’s by no
means complete yet, some results can already be seen.

How to compile
--------------

In contrast to official SMC, I’ve wiped out autohell and converted the
build system to CMake. While still not ideal, it is in any way an
improvement over the overly-complex autotool mess.

### Requirements ###

* Git for checking out the repo.
* An internet connection for the Git checkout.
* A Ruby 1.9 or 2.0 installation with `rake` in your PATH.
* The `gperf` program.
* The `pkg-config` program.
* The `bison` program.
* OpenGL.
* CEGUI 0.7.9. CEGUI >= 0.8 is not supported yet.
* GNU Gettext.
* The FreeImage library.
* The LibPNG library.
* The SDL main library plus SDL_image, SDL_mixer and SDL_ttf,
  all <2.0, which is not supported yet. (Hey, it’s a large codebase
  and I’m working on it alone!)
* The libPCRE regular expression library.

### Checkout the repository ###

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
% git clone git://github.com/Quintus/SMC.git
% cd SMC
% git checkout mruby
% git submodule init
% git submodule update
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Compilation process ###

Execute the following, and read error messages if they appear. Don’t
just go on. They have a sense why they are there.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
% mkdir smc/build
% cd smc/build
% cmake -DCMAKE_INSTALL_PREFIX=$PWD/testinstall \
  -DCMAKE_BUILD_TYPE=Debug ..
% make
% make install
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This will leave you with a complete SMC installation in
smc/build/testinstall. Run the program with:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
% ./testinstall/bin/smc
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

How to compile for Windows (EXPERIMENTAL)
-----------------------------------------

I don’t use Microsoft® Windows®, but thanks to the great guys at
[MinGW](http://www.mingw.org) and those at [MXE](http://mxe.cc) I’ve
made it (**highly experimentally**) possible to crosscompile SMC from
Linux to Windows. To securly provide all required libraries I’ve
forked MXE to https://github.com/Quintus/MXE and created a branch
`smc-building`. You first have to compile MXE in order to compile SMC,
which can easily take a few hours (but you only need to compile MXE
once). 

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
% mkdir ~/smc-building
% cd ~/smc-building
% git clone git://github.com/Quintus/mxe.git
% cd mxe
% git checkout smc-building
% make -j4 cegui libpng freeimage sdl sdl_image sdl_mixer sdl_ttf boost
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This will take a few hours.

Now we have to work around a bug in CMAKE that doesn’t find
`i686-pc-mingw32-pkg-config`, but will only look for `pkg-config`. Do
this:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
% cd usr/bin
% ln -s i686-pc-mingw32-pkg-config pkg-config
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

And then edit `i686-pc-mingw32-pkg-config` with your favourite
editor. Replace the part that says

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
... exec pkg-config --static "$@"
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

with this:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
... exec /usr/bin/pkg-config --static "$@"
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Continue with preparing the build:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
% cd ~/smc-building
% git clone git://github.com/Quintus/SMC.git
% cd SMC
% git checkout mruby
% git submodule init
% git submodule update
% mkdir smc/crossbuild
% cd smc/crossbuild
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Edit the toolhain file smc/cmake/toolchain/linux2mingw32.cmake to
point to your MXE installation, which should be ~/smc-building/mxe if
you followed the above steps. For this, ensure the
`CMAKE_FIND_ROOT_PATH` line is correct:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
set(CMAKE_FIND_ROOT_PATH "$ENV{HOME}/smc-building/mxe")
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Now build SMC in smc/crossbuild.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
% export PATH=$HOME/smc-building/mxe/usr/bin:$PATH
% cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/linux2mingw32.cmake \
  -DCMAKE_INSTALL_PREFIX=$PWD/testinstall -DCMAKE_BUILD_TYPE=Debug ..
% make
% make install
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This will give you a Windows SMC installation in the
crossbuild/testinstall directory. Copy it to Windows or run it with
Wine:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
% wine testinstall/bin/smc.exe
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

As said, this is highly experimental. It may not even build, because I
don’t always test the crosscompilation thing.

Links
-----

* The SMC website: http://www.secretmaryo.org
* The SMC forums: http://secretmaryo.org/phpBB3
* My bugtracker: https://github.com/Quintus/SMC/issues

License
-------

SMC is a two-dimensional jump’n’run platform game.

Copyright © 2003-2011 Florian Richter

Copyright © 2013 Marvin Gülker

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at
your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

_See docs/license.txt for the full license text._
