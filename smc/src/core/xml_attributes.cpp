#include "xml_attributes.hpp"
#include "filesystem/resource_manager.hpp"
#include "property_helper.hpp"

namespace SMC {

	void XmlAttributes::relocate_image(const string& filename_old, const string& filename_new, const string& attribute_name /* = "image" */)
	{
		string current_value = (*this)[attribute_name];
		string filename_old_full = path_to_utf8(pResource_Manager->Get_Game_Pixmaps_Directory() / filename_old);

		if (current_value == filename_old || current_value == filename_old_full)
			(*this)[attribute_name] = filename_new;
	}

	bool XmlAttributes::exists(const string& key)
	{
		if (count(key) > 0)
			return true;
		else
			return false;
	}
}
