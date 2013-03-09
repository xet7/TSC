// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#include "scripting.h"
#include "../level/level.h"
#include "../level/level_player.h"
#include "../core/sprite_manager.h"

#include "objects/mrb_sprite.h"

namespace SMC
{

	namespace Scripting
	{

		ClassMap type2class; // extern
		ClassMap class2type; // extern

		void Initialize_Scripting()
		{
			/* List of classes available in the SMC scripting interface.
			 * TODO: See comments further below for a better approach */
			type2class[typeid(cSprite).name()] = "Sprite";

			// Invert the type2class table
			for (ClassMap::const_iterator iter = type2class.begin(); iter != type2class.end(); iter++)
				class2type[(*iter).second] = (*iter).first;
		}

		cMRuby_Interpreter::cMRuby_Interpreter(cLevel* p_level)
		{
			// Set member variables
			mp_level = p_level;
			mp_mruby = mrb_open();

			// Load our extensions into mruby
			Init_SMC_Libs();

			// Build up the UID table
			cSprite_List::iterator iter;
			cSprite_List& objs = p_level->m_sprite_manager->objects; // Shorthand
			m_uid_table = mrb_hash_new(mp_mruby);

			debug_print("Building UIDS hash\n");
			for(iter = objs.begin(); iter != objs.end(); iter++){
				cSprite* p_sprite = *iter;
				mrb_value sprite = Wrap_MRuby_Object_Around_Sprite(p_sprite);

				mrb_hash_set(mp_mruby, m_uid_table, mrb_fixnum_value(p_sprite->m_uid), sprite);
			}

			// UID 0 is the player
			mrb_value player = Wrap_MRuby_Object_Around_Sprite(pLevel_Player);
			mrb_hash_set(mp_mruby, m_uid_table, mrb_fixnum_value(0), player);
			debug_print("Done building UIDS hash\n");

			// Make the hash globally available
			mrb_define_const(mp_mruby, mp_mruby->object_class, UID_TABLE_NAME.c_str(), m_uid_table);
		}

		cMRuby_Interpreter::~cMRuby_Interpreter()
		{
			mrb_close(mp_mruby);
		}

		mrb_state* cMRuby_Interpreter::Get_MRuby_State()
		{
			return mp_mruby;
		}

		cLevel* cMRuby_Interpreter::Get_Level()
		{
			return mp_level;
		}

		bool cMRuby_Interpreter::Run_Code(const std::string& code, std::string& errormsg)
		{
			mrb_load_string(mp_mruby, code.c_str());
			if (mp_mruby->exc){
				// Format an exception method like this: Message (exception class)
				errormsg = std::string(mrb_string_value_ptr(mp_mruby, mrb_funcall(mp_mruby, mrb_obj_value(mp_mruby->exc), "message", 0)));
				errormsg.append(" (");
				errormsg.append(mrb_obj_classname(mp_mruby, mrb_obj_value(mp_mruby->exc)));
				errormsg.append(")");
				return false;
			}
			else
				return true;
		}

		/* Instead of having a manually-maintained mapping hash of C++-Ruby-class
		 * mappings, we could instead have the cSprite subclasses call out to
		 * these two methods in their constructor and destructor in order to
		 * register an entry in the UIDS hash with the proper class (they know
		 * which class they belong to), freeing us from the need for typeid().
		 * The cSprite constructor itself should not include this behaviour by
		 * default (may be activatable via a compilation option I’d suggest), because
		 * this would cause EVERY sprite in the game to have an associated MRuby object,
		 * even stupid things like particles from particle emitters which are 99% likely
		 * to never be accessed from MRuby, causing a completely unnecessary performance
		 * hit. Instead, if UIDS.[] is called with a not-yet mapped UID, i.e. requesting
		 * a sprite without a mapped subclass, we could simply iterate over the list of
		 * active sprites in the sprite manager, create a MRuby object (class Sprite)
		 * for it, add it to the UIDS hash and then return it. The next request for this
		 * UID will return the cached object. */

		/*void Register_Sprite(cSprite* p_sprite)
		{
			mrb_value sprite = Data_Wrap_Struct(mp_ruby, p_rcSprite, &rtSprite, p_sprite);
			mrb_hash_set(mp_ruby, m_uid_table, mrb_fixnum_value(p_sprite->m_uid, sprite));
			// TODO: Check if the uid is already in use and raise an exception if so!
		}

		void Unregister_Sprite(cSprite* p_sprite)
		{
			mrb_hash_delete_key(	mp_ruby,
									m_uid_table,
									mrb_fixnum_value(p_sprite->m_uid),
									mrb_nil_value());
		} */

		void cMRuby_Interpreter::Init_SMC_Libs()
		{
			Init_Sprite(mp_mruby);
		}

		mrb_value cMRuby_Interpreter::Wrap_MRuby_Object_Around_Sprite(cSprite* p_sprite)
		{
			// TODO: For now, we’re only creating Sprite instances, without
			// regarding subclasses.
			struct RData* sprite = Data_Wrap_Struct(mp_mruby, p_rcSprite, &rtSprite, p_sprite);
			return mrb_obj_value(sprite);
		}

	};

};
