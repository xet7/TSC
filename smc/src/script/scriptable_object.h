// -*- mode: c++; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4 -*-
#ifndef SMC_SCRIPT_SCRIPTABLE_OBJECT_H
#define SMC_SCRIPT_SCRIPTABLE_OBJECT_H
#include <map>
#include <string>
#include <vector>

namespace SMC{
	namespace Script{

		/**
		 * This class encapsulates the stuff that is common
		 * to all objects exposed to the Lua scripting
		 * interface. TODO: Create an Object class in Lua!
		 */
		class cScriptable_Object
		{
		public:
			// Event handlers associated with this object. Format:
			//	 "event name" => {list, of, lua, function, references, to, call}
			std::map<std::string, std::vector<int> > m_event_table;
		};
	};
};
#endif
