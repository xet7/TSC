#ifndef SMC_XML_ATTRIBUTES_H
#define SMC_XML_ATTRIBUTES_H
#include "global_game.h"

namespace SMC {
  class XmlAttributes: public std::map<std::string, std::string>
  {
  public:
    void relocate_image(const std::string& filename_old, const std::string& filename_new, const std::string& attribute_name = "image");
  };
}

#endif
