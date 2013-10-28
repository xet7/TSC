#include "xml_attributes.h"
#include "filesystem/resource_manager.h"
#include "property_helper.h"

using namespace SMC;

void XmlAttributes::relocate_image(const std::string& filename_old, const std::string& filename_new, const std::string& attribute_name /* = "image" */)
{
	std::string current_value = (*this)[attribute_name];
	std::string filename_old_full = path_to_utf8(pResource_Manager->Get_Game_Pixmaps_Directory() / filename_old);

	if (current_value == filename_old || current_value == filename_old_full)
		(*this)[attribute_name] = filename_new;
}

std::string XmlAttributes::fetch(const std::string& key, const std::string& defaultvalue /* = "" */)
{
	if (count(key) > 0)
		return (*this)[key];
	else
		return defaultvalue;
}
