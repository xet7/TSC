// -*- c++ -*-
#ifndef SMC_XML_ATTRIBUTES_H
#define SMC_XML_ATTRIBUTES_H
#include "global_game.h"

namespace SMC {
	class XmlAttributes: public std::map<std::string, std::string>
	{
	public:
		// If the given key `attribute_name' has the value `filename_old'
		//(either with or without the pixmaps dir), replace it with `filename_new'.
		void relocate_image(const std::string& filename_old, const std::string& filename_new, const std::string& attribute_name = "image");

		// If the given `key' exists, return its value. Otherwise return `defaultvalue'.
		std::string fetch(const std::string& key, const std::string& defaultvalue = "");

		// Returns true if the given key exists, false otherwise.
		bool exists(const std::string& key);
	};
}

#endif
