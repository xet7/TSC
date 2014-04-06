#ifndef SMC_SCRIPTING_PARTICLE_EMITTER_HPP
#define SMC_SCRIPTING_PARTICLE_EMITTER_HPP
#include "../../scripting.hpp"

namespace SMC {
	namespace Scripting {
		extern struct RClass* p_rcParticleEmitter;
		void Init_ParticleEmitter(mrb_state* p_state);
	}
}

#endif
