find_path(LIBINTL_INCLUDE_DIR libintl.h)
find_library(LIBINTL_LIBRARY NAMES intl libintl)
set(LIBINTL_LIBRARIES ${LIBINTL_LIBRARY})
set(LIBINTL_INCLUDE_DIRS ${LIBINTL_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LibIntl
  DEFAULT_MSG
  LIBINTL_LIBRARY
  LIBINTL_INCLUDE_DIR)

mark_as_advanced(LIBINTL_INCLUDE_DIR LIBINTL_LIBRARY)
