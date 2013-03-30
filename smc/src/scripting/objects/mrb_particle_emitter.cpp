// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#include "../../video/animation.h"
#include "../../level/level.h"
#include "../../core/sprite_manager.h"
#include "mrb_particle_emitter.h"

/**
 * Class: ParticleEmitter
 *
 * The _ParticleEmitter_ is one of the most complex classes in the
 * API. If you’re not familiar with using regular particle emitters from
 * the normal SMC editor, you probably want to go there first and
 * experiment with them, because everything you need to adjust there
 * needs to be adjusted for dynamically created particle emitters as
 * well--with the difference that in the editor you have a nice UI
 * assisting you, whereelse for the dynamically created particle emitters
 * you have to _know_ what to set and what not to set. Particle emitters
 * can be quite hairy beasts, so I recommend you to often load your level
 * and test what your particle emitter will look like, e.g. by
 * registering for Maryo’s _Jump_ event.
 *
 * Particle emitters can be used in two ways: The usual way, which means
 * periodically issueing a defined number of particles at a time. Such a
 * particle emitter can be created by setting the [emitter’s time to
 * live](#emittertimetolive-1) on that particle emitter. If you want more
 * finergrained control about what is going on, you can ignore that
 * setting and call the [#emit](#emit) method directly. Each call to that
 * method will cause the particle emitter to exactly once emit particles
 * according to its configuration.
 *
 * A good number of setter methods accepts a parameter named `rand` that
 * allows to randomise an otherwise statically set value. For instance,
 * the [#time_to_live=](#timetolive-1) method allows you to define
 * the lifespan of particles emitted at a time. By default, the `rand`
 * modification is 0, i.e. the value is taken as-is. So, if you call
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ruby
 * e.set_time_to_live(10)
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * (where `e` is a `ParticleEmitter` instance), all particles will have
 * the same lifespan of 10 seconds. However, if you pass a second
 * parameter, as in
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ruby
 * e.set_time_to_live(10, 2)
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * then this argument will be taken as a random modifier of the
 * operation. This means that in the above example all particles will
 * have a lifespan of
 *
 *     8 <= l <= 12
 *
 * , where `l` is a single particle’s lifespan. This randomisation is
 * done for all particles, allowing you to create more vidid particle
 * emitters that create particles that don’t all behave the same way.
 *
 * Note that, in contrast to all other objects in SMC, it is possible to
 * set a particle emitter’s [Z coordinate](#z), making it possible to
 * appear in front of Maryo or other sprites.
 *
 * Also note that `ParticleEmitter` is not a subclass of `Sprite` (the
 * particle emitter doesn’t show up on the screen itself, just its
 * emitted particles) and the methods defined there don’t apply here
 * therefore.
*/

using namespace SMC;
using namespace SMC::Scripting;

// Extern
struct RClass* SMC::Scripting::p_rcParticleEmitter = NULL;
struct mrb_data_type SMC::Scripting::rtParticleEmitter = {"ParticleEmitter", NULL};

/**
 * Method: ParticleEmitter#initialize
 *
 *   new( x, y [, width [, height ] ] ) → a_particle_emitter
 *
 * TODO: Docs.
 */
static mrb_value Initialize(mrb_state* p_state,  mrb_value self)
{
	mrb_float x, y;
	mrb_float width = 0.0f;
	mrb_float height = 0.0f;

	mrb_get_args(p_state, "ff|ff", &x, &y, &width, &height);

	cParticle_Emitter* p_emitter = new cParticle_Emitter(pActive_Level->m_sprite_manager);
	p_emitter->Set_Emitter_Rect(x, y, width, height);

	// This is a generated object
	p_emitter->Set_Spawned(true);

	/* Let SMC manage the memory, as with all SMC level objects
	 * This has some side effects: Although we create the object
	 * as per the code above, we don’t have to clean it up, this
	 * is done automatically by SMC when the particle emitter has
	 * finished. It also means that the DATA pointer inside the
	 * MRuby object gets invalid in the instant SMC cleans the
	 * pointer. Not sure if we really need to cater for this, although
	 * it of course leads to segfaults if such an MRuby object is
	 * used...
	 */
	pActive_Animation_Manager->Add(p_emitter);

	DATA_PTR(self) = p_emitter;
	DATA_TYPE(self) = &rtParticleEmitter;

	return self;
}

/**
 * Method: ParticleEmitter#z=
 *
 *   z=( val )
 *
 * TODO: Docs.
 */
static mrb_value Set_Z(mrb_state* p_state,  mrb_value self)
{
	mrb_float z;
	mrb_get_args(p_state, "f", &z);

	cParticle_Emitter* p_emitter = Get_Data_Ptr<cParticle_Emitter>(p_state, self);
	p_emitter->Set_Pos_Z(z);

	return mrb_float_value(z);
}

/**
 * Method: ParticleEmitter#image_filename
 *
 *   image_filename() → a_string
 *
 * TODO: Docs.
 */
static mrb_value Get_Image_Filename(mrb_state* p_state,  mrb_value self)
{
	cParticle_Emitter* p_emitter = Get_Data_Ptr<cParticle_Emitter>(p_state, self);
	return mrb_str_new_cstr(p_state, p_emitter->m_image_filename.c_str());
}

/**
 * Method: ParticleEmitter#image_filename=
 *
 *   image_filename=( path )
 *
 * TODO: Docs.
 */
static mrb_value Set_Image_Filename(mrb_state* p_state,  mrb_value self)
{
	char* str = NULL;
	mrb_get_args(p_state, "z", &str);

	cParticle_Emitter* p_emitter = Get_Data_Ptr<cParticle_Emitter>(p_state, self);
	p_emitter->Set_Image_Filename(str);

	return mrb_str_new_cstr(p_state, str);
}

/**
 * Method: ParticleEmitter#time_to_live
 *
 *   time_to_live() → an_array
 *
 * TODO: Docs.
 */
static mrb_value Get_Time_to_Live(mrb_state* p_state,  mrb_value self)
{
	cParticle_Emitter* p_emitter = Get_Data_Ptr<cParticle_Emitter>(p_state, self);

	mrb_value result = mrb_ary_new(p_state);
	mrb_ary_push(p_state, result, mrb_fixnum_value(p_emitter->m_time_to_live));
	mrb_ary_push(p_state, result, mrb_fixnum_value(p_emitter->m_time_to_live_rand));

	return result;
}

/**
 * Method: ParticleEmitter#set_time_to_live
 *
 *   time_to_live=( ttl )
 *   set_time_to_live ( ttl [, rand ] )
 *
 * TODO: Docs.
 */
static mrb_value Set_Time_to_Live(mrb_state* p_state,  mrb_value self)
{
	mrb_float time;
	mrb_float rand = 0.0f;
	mrb_get_args(p_state, "f|f", &time, &rand);

	cParticle_Emitter* p_emitter = Get_Data_Ptr<cParticle_Emitter>(p_state, self);
	p_emitter->Set_Time_to_Live(time, rand);

	return mrb_nil_value();
}

void SMC::Scripting::Init_ParticleEmitter(mrb_state* p_state)
{
	p_rcParticleEmitter = mrb_define_class(p_state, "ParticleEmitter", p_state->object_class);
	MRB_SET_INSTANCE_TT(p_rcParticleEmitter, MRB_TT_DATA);

	// Methods
	mrb_define_method(p_state, p_rcParticleEmitter, "initialize", Initialize, ARGS_REQ(2) | ARGS_OPT(2));
	mrb_define_method(p_state, p_rcParticleEmitter, "z=", Set_Z, ARGS_REQ(1));
	mrb_define_method(p_state, p_rcParticleEmitter, "image_filename", Get_Image_Filename, ARGS_NONE());
	mrb_define_method(p_state, p_rcParticleEmitter, "image_filename=", Set_Image_Filename, ARGS_REQ(1));
	mrb_define_method(p_state, p_rcParticleEmitter, "time_to_live", Get_Time_to_Live, ARGS_NONE());
	mrb_define_method(p_state, p_rcParticleEmitter, "set_time_to_live", Set_Time_to_Live, ARGS_REQ(1) | ARGS_OPT(1));

	// Aliases
	mrb_define_alias(p_state, p_rcParticleEmitter, "time_to_live=", "set_time_to_live");
}
