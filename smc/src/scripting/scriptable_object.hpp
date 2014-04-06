// -*- c++ -*-
#ifndef SMC_SCRIPTING_SCRIPTABLE_OBJECT_H
#define SMC_SCRIPTING_SCRIPTABLE_OBJECT_H
#include "../core/global_basic.hpp"

namespace SMC{
	namespace Scripting{

		/**
		 * This class encapsulates the stuff that is common
		 * to all objects exposed to the mruby scripting
		 * interface. That, it holds the mruby event tables.
		 */
		class cScriptable_Object
		{
		public:
			cScriptable_Object(){}
			virtual ~cScriptable_Object(){}

			std::map<std::string, std::vector<mrb_value> > m_callbacks;
		};
	};
};
#endif
