// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#ifndef SMC_SCRIPT_DIE_H
#define SMC_SCRIPT_DIE_H
#include <string>
#include "../script.h"
#include "event.h"
namespace SMC{
	namespace Script{
		class cGold_100_Event: public cEvent
		{
		public:
			virtual std::string Event_Name(){return "gold_100";}
		};
	};
};
#endif
