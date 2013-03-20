// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#include "mrb_uids.h"
#include "../../level/level.h"
#include "../../objects/sprite.h"
#include "../../core/sprite_manager.h"

/*****************************************************************************
 Sprite management

Each active sprite in SMC is assigned a unique number, the UID, available
via the `m_uid' member of cSprite instances (sprites are considered active
if they belong to the cSprite_Manager instance of a cLevel). By default,
these sprites have no associated MRuby objects, allowing a fast level start.
However, as soon as the user starts to index the global `UIDS' object, things
change: Each call to UIDS::[] with a not-yet encountered valid UID (i.e. a
UID actually belonging to an active sprite) will create an MRuby object
wrapping the specific cSprite instance and return it. Additionally, this
MRuby object is cached in an internal cache `s_uids_cache' below so
that further calls to UIDS::[] with the same UID will actually return
the same object (this is very important for event handling). The MRuby
object will continue to exist until the sprite goes inactive, i.e. is
removed from the cSprite_Manager instance, which requests the cache to
delete that specific UID via Delete_UID_From_cache().

There is no static mapping between the C++ cSprite subclasses and the
MRuby Sprite subclasses. Instead, each cSprite subclass (and cSprite
itself) defines a virtual method Create_MRuby_Object() which is supposed
to create an MRuby object of the proper class for the cSprite subclass
instance itself, so that e.g. cEato can create an instance of the Eato
class rather than just an instance of Enemy. Having static lookup tables
for this is bad style, and the method apporach additionally allows you
to “hide” an object from the UIDS hash by just returning mrb_nil_value().
When UIDS::[] is called with a not-yet encountered valid UID as described
above, this will result in a call to the Create_MRuby_Object() method of
the sprite corresponding to the passed UID.

Note that adding the MRuby object directly to the sprite (thus
creating a circular reference between the two) is a bad idea, because
MRuby’s GC won’t know about the C++-side reference. It would just see
an object referenced from nowhere in the Ruby environment, and collect
it. When later the user requests this object, it will not be there,
causing a segfault. Sttoring the MRuby instances in the global constat
`UIDS' ensures that the GC knows about them and won’t collect them.

*****************************************************************************/

using namespace SMC;

// Extern
struct RClass* SMC::Scripting::p_rmUIDS = NULL;

static mrb_value Index(mrb_state* p_state, mrb_value self)
{
	mrb_value ruid;
	mrb_get_args(p_state, "o", &ruid);
	mrb_int uid = mrb_fixnum(ruid);

	// Try to retrieve the sprite from the cache
	mrb_value cache  = mrb_iv_get(p_state, self, mrb_intern(p_state, "cache"));
	mrb_value sprite = mrb_hash_get(p_state, cache, ruid);

	// If we already have an object for this UID in the
	// cache, return it.
	if (!mrb_nil_p(sprite))
		return sprite;

	// Otherwise, allocate a new MRuby object for it and store
	// that new object in the cache.
	cSprite_List objs = pActive_Level->m_sprite_manager->objects; // Shorthand
	for(cSprite_List::const_iterator iter = objs.begin(); iter != objs.end(); iter++){
		if ((*iter)->m_uid == uid) {
			// Ask the sprite to create the correct type of MRuby object
			// so we don’t have to maintain a static C++/MRuby type mapping table
			mrb_value obj = (*iter)->Create_MRuby_Object(p_state);
			mrb_hash_set(p_state, cache, ruid, obj);

			return obj;
		}
	}

	return mrb_nil_value();
}

// FIXME: Call Scripting::Delete_UID_From_Cache for sprites
// being removed from a level’s cSprite_Manager!
void SMC::Scripting::Delete_UID_From_Cache(mrb_state* p_state, int uid)
{
	mrb_value cache = mrb_iv_get(p_state, mrb_obj_value(p_rmUIDS), mrb_intern(p_state, "cache"));
	mrb_hash_delete_key(p_state, cache, mrb_fixnum_value(uid));
}

void SMC::Scripting::Init_UIDS(mrb_state* p_state)
{
	p_rmUIDS = mrb_define_module(p_state, "UIDS");

	// Create a `cache' instance variable invisible from Ruby.
	// This is where the cached sprite instances will be stored,
	// visible for the GC.
	mrb_value cache = mrb_hash_new(p_state);
	mrb_iv_set(p_state, mrb_obj_value(p_rmUIDS), mrb_intern(p_state, "cache"), cache);

	// UID 0 is always the player
	mrb_hash_set(p_state, cache, mrb_fixnum_value(0), mrb_const_get(p_state, mrb_obj_value(p_state->object_class), mrb_intern(p_state, "Player")));

	mrb_define_class_method(p_state, p_rmUIDS, "[]", Index, ARGS_REQ(1));
}
