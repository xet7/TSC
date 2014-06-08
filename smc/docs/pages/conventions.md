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

State of transition
-------------------

SMC’s codebase has seen different coding styles in the past. You will
find that most of SMC does not correspond to this styleguide, but we
are working on it. Some day we will have it done...
