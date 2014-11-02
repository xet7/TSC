Coding conventions
==================

This file documents the coding conventions used in SMC. If you plan on
contributing to the game, please read this.

The general style used for SMC’s codebase is the [Stroustrup
style][1]. For any formatting questions, you should therefore read
that document prior to consulting this one, which outlines the
additions and differences to the standard Stroustrup style.

Note that for Emacs an appropriate style file is already provided in
the source tree, `smc/.dir_locals.el`. That file will automatically be
picked up by Emacs when you edit the sourcecode, so you don’t have to
worry about changing the indentation and other style settings
manually. Stil, a style file doesn’t lift the burden of properly
formatting the code, it is just a help.

File names
----------

Sourcecode files end in `.cpp`, header files end in `.hpp` in case of
C++ code (which should be everything in SMC). Use `.c` and `.h` if for
some unknown reason pure C code is required.

Indentation
-----------

Code in SMC is indented with 4 spaces, no tabs. Most code editors can
be configured to automatically insert the required number of spaces
when you hit the TAB key.

~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
void foo()
{
    cool_code();
}
~~~~~~~~~~~~~~~~~~~~~~~~~~

License header
--------------

Each file in SMC is required to start with the following license
header:

~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
/***************************************************************************
* <FILENAME>.<cpp/hpp> - <SHORT DESCRIPTION OF THE FILE>
*
* Copyright © <YEAR> The SMC Contributors
***************************************************************************
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
~~~~~~~~~~~~~~~~~~~~~~~~~~

If you contribute to SMC the first time, don’t forget to include
yourself into `docs/authors.txt` so you show up in the credits!

Class names
-----------

As a difference to Stroustrup style, due to historical reasons class
names start with a lowercase c, following by the real name in
Camel_Case.

~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
class cFoo_Bar {
    // ...
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Nesting classes is OK for indicating a very strong relationship
between two classes.

~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
class cPath {

    class cSegment {
    };

};
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Do not repeat the outer class name in the inner class’ name, it’s
obvious from the namespacing (`cPath::cPathSegment` is **wrong**,
`cPath::cSegment` is correct). Do not nest further than one level
unless absolutely required.

Methods
-------

Another difference to Stroustrup style is method naming, again for
historical reasons. Methods in classes are uppercase, both for member
and static methods, and long names use Camel_Case. Class-independent
methods (i.e. utility methods) are lowercase.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
class cFoo {
public:
    void Cool_Method();
};

void utility_method();
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Variables and parameters
------------------------

Variable and parameter names use an abbreviated hungarian
notation.

* For local throw-away variables just use a descriptive name,
  don’t apply any prefixes.
* Member variables are prefixed with "m_"
* Static member variables are prefixed with "s_"
* Global variables are prefixed with "g_"

In any case, a variable for a pointer should always contain
"p". Prefixes are combinable. Examples:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
int count; // local variable
int m_count; // member variable
static int s_count; // static member variable
extern int g_count; // global variable
int* p_count; // local-var pointer
int* mp_count; // member-var pointer
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Variable access
---------------

Usually member variables should be either `private` or `protected`,
and getter and setter methods should be defined to access these
variables. This way, you can be sure in the implementation of your
class that nobody messes with the variables from the outside in an
unexpected way.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
class cFoo {
public:
    int Get_Bar();
    void Set_Bar(int x);

private:
    int m_bar;
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Additional method calls have performance overhead. Most getters and
setters are trivial, so you want to squash the performance overhead by
taking advantage of C++’s `inline` keyword that retains the secrecy
principle and still allows the compiler to optimize the method call
away:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
class cFoo {
public:
    inline int Get_Bar(){ return m_bar; }
    inline void Set_Bar(int x){ m_bar = x; }

private:
    int m_bar;
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Only do this for small one-line accessor methods, otherwise
you abuse the `inline` keyword.

Macros
------

Try to prevent macro use as much as possible. C++ has templates that
will cover a good number of macro usecases, but sometimes you just
can’t avoid a macro (such as the one defining the event handler
callback for mruby (a C library!) functions; doing that without the
`MRUBY_IMPLEMENT_EVENT()` macro would be much more work and much more
unclear than this way). If you come to the conclusion you absolutely
must use a macro, make it use a name IN_ALL_CAPS so it stands out of
the code and warns everyone it is a macro.

The header guards that prevent `#include` from including a header file
multiple times are of course an exception. Header guard macros in SMC
should always look like this:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
#ifndef SMC_NAMESPACE_CLASS_HPP
#define SMC_NAMESPACE_CLASS_HPP
// Code...
#endif
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Do not indent `#ifdef` and `#if defined` conditions. They are not part
of the code and must quickly be found from just glancing over the file
when you want to know whether you are in a Windows- or Linux-specific
part of the code.

Implementing code that needs to differenciate between multiple
different operating systems (or other environment-related macros)
should look like this:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
void foo()
{
#if defined(_WIN32)
    windows_specific_stuff();
    with_another_line();
#elsif defined(__linux)
    linux_specific_stuff();
#else
#error unsupported platform
#endif
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Be sure to always add an `#else` part containing an `#error` line so
that users that want to compile on an unhandled platform receive a
compilation error rather than a dysfunctional program.

Comments
--------

Comments should be added as suggested in the Stroustrup style
document, i.e. where you have to say things that you can’t express in
code. Comments that repeat what the code is saying already are bad,
especially if they get out of sync with the code itself.

Single and two-line comments use `//`. For comments with three and
more lines, use `/* */`.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
// Short comment

/* Long comment that explains a more complicated factum that
 * definitely needs more than three lines, because the explanation
 * is so insanely complicated that each less line would prevent
 * the understanding.
 */
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Comments should not exceed a line-length of 80 characters, try to
break at column 60. Use proper English grammar and punctuation. Short
oneline comments may ommit the final period.

Namespace resolution
--------------------

Don’t leave spaces around the `::` operator, neither in a definition
nor in the implementation.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
cFoo :: Bar() // WRONG
{
}

cFoo::Bar() // correct
{
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

External includes
-----------------

All external `#include` statements should be in
`core/global_basic.hpp`. Do not include external headers in other
files. Background for this is that in some cases the order of includes
is important, which cannot easily be guaranteed when headers include
other headers of SMC, which include yet another SMC header, which then
finally includes some library header, etc. `#include` statements for
SMC-own headers can be done anywhere you need them.

SMC namespace
-------------

All SMC code is required to be defined under the `SMC` namespace. In
the header files, always write out `namespace SMC {` in full and
indent accordingly. In your sourcecode files, add a line

~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
using namespace SMC;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

near the top so you don’t have to indent all your implementation
code. Please do **not** follow the old SMC style of using `namespace
SMC {` and not indenting the rest of the file, this is highly
discouraged.

Version policy
--------------

SMC uses [semantic versioning](http://semver.org/). In short this
means:

* Version numbers are triplets of form `MAJOR.MINOR.TINY`.
* `TINY` is increased for bugfixes, internal code restructuring, etc.
* `MINOR` is increased when new features are added that maintain
  backward compatibility (especially with regard to level and save
  file formats).
* `MAJOR` is increased when backward compatibility is broken.

Additionally, each compiled executable of SMC knows about the exact
commit’s hash it was compiled from. Execute SMC like this to receive
the full version information:

~~~~~~~~~~~~~~~~~~~~~~~~~~
$ smc --version
~~~~~~~~~~~~~~~~~~~~~~~~~~

Branching
---------

The version control system (VCS) in use for SMC is
[Git](http://www.git-scm.com/), and the repository currently resides
at [GitHub](https://github.com/Secretchronicles/SMC). For managing
these sources, we generally follow the so-called [“Git
Flow”](http://nvie.com/posts/a-successful-git-branching-model/) model,
which heavily relies on Git’s lightweight branches and in a nutshell
means the following for SMC:

* Each commit on `master` is a final release and is tagged
  accordingly.
* The development of the next feature release happens in `devel`.
* If a regular release is about to be shipped, a branch
  `release-X.Y.Z` is created, where `X.Y.Z` is the version
  number. This branch only receives bugfixes, and it is merged every
  now and then into `devel` to make it also receive the bugfixes.
* Larger new features in development are branched off `devel` into a
  branch named `feature-something`, where `something` is a short
  description of the feature. `devel` should be merged every now
  and then into the feature branch. When the feature is completed,
  it is merged back into `devel` (unless it breaks compatibility, see
  below).

Beware the following exception, which is a **difference to official
GitFlow**: As we use semantic versioning (see the preceeding section),
we must be careful as to what to merge into `devel` as `devel` will
automatically become the codebase for the next release. Semantic
versioning however requires us to handle features or other changes
that break backward compatibility specifically, they are not allowed
to just go into the next release, i.e. into `devel`, so that the user
is able to derive compatibility information from SMC’s version
number. To prevent feature branches from getting silently out-of-date
when they are completed and not merged into devel, we maintain
specific `devel-X.0.0` branches that serve the purpose of the `devel`
branch for the next **breaking** release. The lifecycle of a feature
branch whose feature breaks backward compatibility hence looks like
this:

1. If you know from the beginning your feature will break backward
  compatibility, branch off `devel-X.0.0` a branch named
  `bfeature-something` (note the leading `b`). If you don’t know that,
  or if there is no `devel-X.0.0` branch yet, branch off `devel`. Once
  you know your feature will break, correct your branch name (by
  creating a new `bfeature` branch and merging your `feature` branch
  into it, then deleting the `feature` branch).
2. Make your development in the `bfeature` branch, occasionally
  merging `devel-3.0.0`, or if that doesn’t exist, `devel` into your
  branch.
3. Once the breaking feature is completed, merge it into
  `devel-X.0.0`. If there is no `devel-X.0.0` branch at that time,
  punch the project lead until he creates one.

Occasionally, `devel` will be merged into `devel-X.0.0`, but this
doesn’t have to be done by you. Also, when the breaking major release
is finally due, `devel-X.0.0` will be merged into `devel`, as it then
constitutes the next release anyway.

If you feel the need to discuss the code you wrote, be it a feature or
a bugfix, open a pull request on GitHub even if you have write access
to the repository; GitHub allows pull requests in the same
repository. If your code is not ready yet, prepend a prominent “[WIP]” (=
Work In Progress) mark to the title.

Documentation
-------------

SMC uses [Doxygen][2] for documentation. Inside the header files,
please only provide Doxygen "brief" descriptions of the elements (if
any) in order to not clutter the header files, which this way can be
used as a short quick-reference (Doxygen uses the "brief" comment for
display in the function summary). For the more complete descriptions,
use doxygen "full" documentation comments around the actual
definitions in the `.cpp` files. Doxygen comment style is `/**` for
full comment blocks, and `///` for brief comments.

Example `foo.hpp`:

\verbatim
class cFoo
{
   /// Does important things.
   void Bar();
};
\endverbatim

Example `foo.cpp`:

\verbatim
/**
 * This method does important things by
 * implementing the XYZ pattern. It
 * can be used...
 */
void cFoo::Bar()
{
    // Code...
}
\endverbatim

Example of a fully documented function:

\verbatim
// .hpp file
class cFoo {
public:
    /// Do important things with the position.
    Bar(int x, int y, int count);
};
\endverbatim

\verbatim
// .cpp file

/**
 * This method does important things with
 * the given position.
 *
 * \param x     The X coordinate.
 * \param y     The Y coordinate.
 * \param count The number of times to do it.
 *
 * \returns whether the action was successful.
bool cFoo::Bar(int x, int y, int count)
{
    // Code...
}
\endverbatim

Miscellanea
-----------

* Use `true` and `false` instead of `1` and `0`.
* A function should ideally fit onto a single screen page. While this
  is not always possible (both due to logical code requirements and
  different screen sizes), you should ask yourself if refactoring is
  needed when your function gets a thousand lines long.

State of transition
-------------------

SMC’s codebase has seen different coding styles in the past. You will
find that most of SMC does not correspond to this styleguide, but we
are working on it. Some day we will have it done...

[1]: http://www.stroustrup.com/Programming/PPP-style-rev3.pdf
[2]: http://www.stack.nl/~dimitri/doxygen
