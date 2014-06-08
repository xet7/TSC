Sprite management in the scripting API
======================================

Each active sprite in SMC is assigned a unique number, the UID, available
via the `m_uid` member of cSprite instances (sprites are considered active
if they belong to the cSprite_Manager instance of a cLevel). By default,
these sprites have no associated MRuby objects, allowing a fast level start.
However, as soon as the user starts to index the global `UIDS` object, things
change: Each call to `UIDS::[]` with a not-yet encountered valid UID (i.e. a
UID actually belonging to an active sprite) will create an MRuby object
wrapping the specific cSprite instance and return it. Additionally, this
MRuby object is cached in an internal `cache` module instance variable so
that further calls to `UIDS::[]` with the same UID will actually return
the same object (this is very important for event handling). The MRuby
object will continue to exist until the sprite goes inactive, i.e. is
removed from the cSprite_Manager instance, which requests the cache to
delete that specific UID via `SMC::Scripting::Delete_UID_From_cache()`.

There is no static mapping between the C++ cSprite subclasses and the
MRuby Sprite subclasses. Instead, each cSprite subclass (and cSprite
itself) defines a virtual method `Create_MRuby_Object()` which is supposed
to create an MRuby object of the proper class for the cSprite subclass
instance itself, so that e.g. cEato can create an instance of the Eato
class rather than just an instance of Enemy. Having static lookup tables
for this is bad style, and the method apporach additionally allows you
to “hide” an object from the UIDS hash by just returning mrb_nil_value().
When `UIDS::[]` is called with a not-yet encountered valid UID as described
above, this will result in a call to the Create_MRuby_Object() method of
the sprite corresponding to the passed UID.

Note that adding the MRuby object directly to the sprite (thus
creating a circular reference between the two) is a bad idea, because
MRuby’s GC won’t know about the C++-side reference. It would just see
an object referenced from nowhere in the Ruby environment, and collect
it. When later the user requests this object, it will not be there,
causing a segfault. Sttoring the MRuby instances in the global constant
`UIDS' ensures that the GC knows about them and won’t collect them.
