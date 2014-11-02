Scripting implementation
========================

Yeah, scripting! SMC embeds mruby (https://github.com/mruby/mruby), a
minimal Ruby implementation for boosting your level design. The
cMRubyInterpreter class wraps the `mruby_state` struct in a more
convenient way and also initialises it for use with SMC. Each time a
level is loaded, a new clean instance of this class is created (and of
course destroyed on level ending).

The central point of scripting is the cMRuby_Interpreter class, which
wraps mruby’s `mrb_state` pointer. After the setup is done, the
constructor calls the cMRuby_Interpreter::Load_Scripts() private
member which in turn feeds the `main.rb` file in the `scripting/`
directory of your SMC installation into the mruby interpreter. This
script, which may be added by users if they want to provide additional
functionality globally, has access to a very minimalistic version of
`#require`, which just allows for loading scripts relative to the
scripting/ directory and also has some quirks, such as the inability
to define toplevel constants without an explicit :: at the beginning,
e.g.

~~~~~~~~~~~~~~~~~~~~ ruby
module Foo
end
~~~~~~~~~~~~~~~~~~~~

will not work, but

~~~~~~~~~~~~~~~~~~~~ ruby
module ::Foo
end
~~~~~~~~~~~~~~~~~~~~

will. The default `main.rb` file only contains the bare minimum needed
to correctly initialise the scripting functionality and users are
advised to not remove this code. Removing e.g. the `Eventable` module
will have very bad effects, but on the other hand this gives you the
full power of Ruby to hook in everything you want to. Finally, the
`main.rb` script is *required* to call the `SMC::setup` method, which
loads all the C++ wrapper classes (i.e.  Sprite, LevelPlayer, etc.)
into the interpreter.

mruby type
----------

All objects that are scriptable in SMC inherit from `cScriptable_Object`
on the C++ side of things. This class is responsible for holding the
event tables, and is the most basic class you can cast to when you
don’t know what to retrieve out of an mruby object.

mruby wraps a pointer to some C++ object which can be extracted by
means of the `Get_Data_Ptr()` function (defined in
scripting/scripting.hpp). Note that mruby usually carries around type
information in form of a special object (`mrb_data_type`) it uses to
check when getting out the underlying pointer of the object with
mruby’s own retrieval method (`Data_Get_Struct`), but as mruby was
mainly written for C and not for C++, this badly fails for inheritance
and hence can’t be used in SMC. Therefore, we tell mruby whenever
creating a new mruby class that its "C" pointer type is
`SMC::Scripting::rtSMC_Scriptable`, so we effectively ommit this
checking layer of mruby. You have to be careful therefore to specify
the correct type for `Get_Data_Ptr()`.

As a consequence, to get the mapping information correct on the mruby
side, each C++ class is required to implement a
`Create_MRuby_Object()` method that is called whenever this C++ object
needs to be accessed in mruby. It usually looks like this (example
taken from eato.hpp):

~~~~~~~~~~~~~~~~~~~~ c++
virtual mrb_value Create_MRuby_Object(mrb_state* p_state)
{
	return mrb_obj_value(Data_Wrap_Struct(p_state, Scripting::p_rcEato, &Scripting::rtSMC_Scriptable, this));
}
~~~~~~~~~~~~~~~~~~~~

Be sure to specify the correct mruby class (the `p_rc` thing).

Note that from C++ *all* mruby objects have a single C type:
`mrb_value`. You must use the functions provided by mruby to
manipulate these objects. You don’t have to know how `mrb_value` works
internally, suffice it to say that `mrb_value` is effectively a
pointer, so copying it has no performance impact compared to copying a
pointer. Do not use `mrb_value*` therefore without reason.

Defining classes
----------------

For each mruby class you want to define, you have to declare and fill
a pointer of type `RClass*`, which as a convention we always name
`p_rcClass_Name`. Along with this you need to provide an
`Init_ClassName` function that you need to call inside the scripting
initializing sequence by adding it to
`SMC::Scripting::Load_Wrappers()` in scripting.cpp. Your header for a
new mruby class will therefore look like this (example taken from
eato):

~~~~~~~~~~~~~~~~~~~~ c++
#ifndef SMC_SCRIPTING_EATO_HPP
#define SMC_SCRIPTING_EATO_HPP
#include "../../scripting.hpp"

namespace SMC {
	namespace Scripting {
		extern struct RClass* p_rcEato;
		void Init_Eato(mrb_state* p_state);
	}
}
#endif
~~~~~~~~~~~~~~~~~~~~

The `Init_Eato()` method is now required to:

1. Define the new mruby class `Eato`
2. Tell mruby instances of this class wrap a pointer
3. Define the mruby methods for this class

An mruby class is defined with `mrb_define_class`, telling mruby
objects of this class wrap a pointer can be done with mruby’s
`MRB_SET_INSTANCE_TT` macro.

~~~~~~~~~~~~~~~~~~~~ c++
// "Eato" is the name of the class.
// "p_rcEnemy" is the parent mruby (!) class
p_rcEato = mrb_define_class(p_state, "Eato", p_rcEnemy);
MRB_SET_INSTANCE_TT(p_rcEato, MRB_TT_DATA);
~~~~~~~~~~~~~~~~~~~~

Defining methods
----------------

Defining a method is possible with `mrb_define_method()`. Check the
existing sourcefiles for various examples; note that the special
method named `initialize` in mruby is called automatically after an
object has been instanciated (it therefore resembles the constructor
in C++ terms). Each mruby method has the very same signature in C++:

~~~~~~~~~~~~~~~~~~~~ c++
static mrb_value Get_Image_Dir(mrb_state* p_state,  mrb_value self);
~~~~~~~~~~~~~~~~~~~~

You will in 99% of the cases only want your mruby method to be
accessible in C++ in the current file, so declare these methods as
file-local by using the `static` keyword. The first argument for this
method is the mruby interpreter state, the second one — commonly named
`self` — is the object the method is being called on (it’s the same
concept as with C++’s `this`). Using `Get_Data_Ptr()`, you can unwrap
the underlying C++ pointer from this object.

~~~~~~~~~~~~~~~~~~~~ c++
cEato* p_eato = Get_Data_Ptr<cEato>(p_state, self);
~~~~~~~~~~~~~~~~~~~~

Note that of course you can’t do this in `initialize`, because no C++
object has been wrapped yet when that method is called. Instead, you
first have to create the C++ object as usual, and then employ mruby’s
`DATA_PTR` and `DATA_TYPE` macros to wrap it into the mruby value.

~~~~~~~~~~~~~~~~~~~~ c++
DATA_PTR(self) = p_eato;
DATA_TYPE(self) = &rtSMC_Scriptable; // See above for explanation
~~~~~~~~~~~~~~~~~~~~

Also, you usually want to call `Set_Spawned(true)` on the C++ object,
so that it doesn’t get stored in the level file when the level
editor’s save function is invoked by the user.

str2sym
-------

Often you need to directly convert a C++ string into an mruby symbol
object. mruby has no method to directly do this (or even for C
strings), so SMC provides its own: SMC::Scripting::str2sym(). Feel
free to use it.

Events
------

Several objects in SMC allow you to spawn events that invoke callbacks
in mruby land. The event table is managed as a C++ `std::map`, which
maps the name of the event to a `std::vector` of all registered mruby
callbacks (which are just normal mruby objects, i.e. of type
`mrb_value`). This event table is hold in cScriptable_Object as an
internal member variable you can interact with by means of the public
methods defined in cScriptable_Object.

Each event is required to have a C++ class that inherits from
SMC::Scripting::cEvent and at least overwrites the `Event_Name()`
method in such a way that it returns a *unique* name for this event
that will be used as the key for the event table. If you want to pass
custom arguments to the mruby callback, you can also overwrite the
`Run_Mruby_Callback()` method. For passing arguments through from the
instanciation of the event, overwrite the constructor.

To actually fire an event, just go to the place in the C++ code where
you want to fire it, instanciate the event class you just defined, and
call the `cEvent::Fire()` method.

This is basically everything you need, your event will now be availble
via the SMC::Eventable module’s `#bind` method. To ease things for
users, you can use the `MRUBY_IMPLEMENT_EVENT` and
`MRUBY_EVENT_HANDLER` macros declared in scripting/events/event.hpp
that wrap the call to `#bind` for you in an own method named
`on_<evtname>` and return that name as a string, respectively.

See also
--------

* \ref mrubysprites "Sprite management in the scripting API"
