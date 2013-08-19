find_path(FreeImage_INCLUDE_DIR FreeImage.h)
find_library(FreeImage_LIBRARY freeimage)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(FreeImage DEFAULT_MSG FreeImage_INCLUDE_DIR FreeImage_LIBRARY)
set(FreeImage_LIBRARIES ${FreeImage_LIBRARY})
set(FreeImage_INCLUDE_DIRS ${FreeImage_INCLUDE_DIR})
