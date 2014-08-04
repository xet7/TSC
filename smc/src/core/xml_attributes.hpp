#ifndef SMC_XML_ATTRIBUTES_HPP
#define SMC_XML_ATTRIBUTES_HPP
#include "global_game.hpp"
#include "errors.hpp"
#include "property_helper.hpp"

namespace SMC {

	class XmlAttributes: public std::map<string, string>
	{
	public:
		// If the given key `attribute_name' has the value `filename_old'
		//(either with or without the pixmaps dir), replace it with `filename_new'.
		void relocate_image(const string& filename_old, const string& filename_new, const string& attribute_name = "image");

		// Returns true if the given key exists, false otherwise.
		bool exists(const string& key);

		// If the given `key' exists, return its value. Otherwise return `defaultvalue'.
		// For strings, an this template is overriden to do no conversion at all.
		template <typename T>
		T fetch(const string& key, T defaultvalue)
		{
			if (exists(key))
				return string_to_type<T>((*this)[key]);
			else
				return defaultvalue;
		}

		// If the given `key' exists, returns its value as the
		// type indicated by the template. If it doesn’t exist,
		// throw an instance of
		template <typename T>
		T retrieve(const string& key)
		{
			if (exists(key))
				return string_to_type<T>((*this)[key]);
			else
				throw(XmlKeyDoesNotExist(key));
		}
	};

	template<>
	inline string XmlAttributes::fetch(const string& key, string defaultvalue)
	{
		if (exists(key))
			return (*this)[key];
		else
			return defaultvalue;
	}

	template<>
	inline const char* XmlAttributes::fetch(const string& key, const char* defaultvalue)
	{
		if (exists(key))
			return (*this)[key].c_str();
		else
			return defaultvalue;
	}
}

#endif
