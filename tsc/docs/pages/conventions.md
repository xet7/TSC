Coding conventions
==================

This file documents the coding conventions used in SMC. If you plan on
contirbuting to the game, please read this.

Indentation
-----------

Code in SMC is indented with tabs. One tab spans 4 spaces.

~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
void foo()
{
	cool_code();
}
~~~~~~~~~~~~~~~~~~~~~~~~~~

Classes
-------

Class names start with a lowercase c, following by the real name in
Camel_Case.

~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
class cFoo_Bar
{
	// ...
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~

The opening and closing brace for the class definition have its own line.

Methods
-------

Methods in classes are uppercase, both for member and static
methods, and long names use Camel_Case. Class-independent methods
(i.e. utility methods) are lowercase

~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
class cFoo
{
public:
	void Cool_Method();
};

void utility_method();
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

When defining methods, the opening and closing braces are on its own
line.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
void utility_method()
{
	//
}
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

The `*` that determines a pointer is considered to be part of the
type, and not of the name, so it should come *before* the space
between type and name of a variable. The same goes for the `&` for
reference variables.

Control structures
------------------

`if`, `for`, etc. are so common that having their opening braces on an
own line is unnessesarily verbose. Just place them on the line the
control structure is on:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
if (condition) {
  // stuff
}

for(int i=0; i<x; i++) {
  // stuff
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

You may ommit braces for one-line control structures. But do *not* do
this if you place a comment between condition and structure body, even
though this is technically possible. It destroys overview.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
if (condition)
  code();

for(i=0; i<x; i++)
  code();

// WRONG!
if (condition)
  // Long comment to explain this
  // call that distracts the user
  // and makes the code hard to
  // understand.
  code();
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Variable access
---------------

Usually member variables should be either `private` or `protected`,
and getter and setter methods should be defined to access these
variables. This way, you can be sure in the implementation of your
class that nobody messes with the variables from the outside in an
unexpected way.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
class cFoo
{
public:
  int Get_Bar();
  void Set_Bar(int x);

private:
  int m_bar;
};
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

SMC uses [Doxygen](http://www.stack.nl/~dimitri/doxygen) for
documentation. Inside the header files, please only provide brief
descriptions of the elements (if any) in order to not clutter the
header files, which this way can be used as a short
quick-reference. For the more complete descriptions, use doxygen
documentation comments around the actual definitions in the `.cpp`
files.

Example `foo.hpp`:

~~~~~~~~~~~~~~~~~~~~~~~~ c++
class cFoo
{
  /// Does important things.
  void Bar();
}
~~~~~~~~~~~~~~~~~~~~~~~~

Example `foo.cpp`:

~~~~~~~~~~~~~~~~~~~~~~~~ c++
/**
 * This method does important things by
 * implementing the XYZ pattern. It
 * can be used...
 */
void cFoo::Bar()
{
  // Code...
}
~~~~~~~~~~~~~~~~~~~~~~~~

State of transition
-------------------

SMC’s codebase has seen different coding styles in the past. You will
find that most of SMC does not correspond to this styleguide, but we
are working on it. Some day we will have it done...
