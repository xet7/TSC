// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#ifndef SMC_SCRIPTING_PARTICLE_EMITTER_H
#define SMC_SCRIPTING_PARTICLE_EMITTER_H
#include "../scripting.h"

namespace SMC {
	namespace Scripting {
		extern struct RClass* p_rcParticleEmitter;
		extern struct mrb_data_type rtParticleEmitter;
		void Init_ParticleEmitter(mrb_state* p_state);
	}
}

#endif
