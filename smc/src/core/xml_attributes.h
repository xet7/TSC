#ifndef SMC_XML_ATTRIBUTES_H
#define SMC_XML_ATTRIBUTES_H
#include "global_game.h"

namespace SMC {
  class XmlAttributes: public std::map<std::string, std::string>
  {
    void Relocate_Image(const std::string& filename_old, const std::string& filename_new, const std::string& attribute_name = "image");
  };
}

#endif
