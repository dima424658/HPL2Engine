find_path(Ogg_INCLUDE_DIR
  NAMES
    ogg/ogg.h
  DOC "ogg include directory")
mark_as_advanced(Ogg_INCLUDE_DIR)

find_library(Ogg_LIBRARY
  NAMES
    ogg
  DOC "ogg library")
mark_as_advanced(Ogg_LIBRARY)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Ogg REQUIRED_VARS Ogg_LIBRARY Ogg_INCLUDE_DIR)

if (Ogg_FOUND)
  set(Ogg_LIBRARIES "${Ogg_LIBRARY}")
  set(Ogg_INCLUDE_DIRS "${Ogg_INCLUDE_DIR}")

  if (NOT TARGET Ogg::ogg)
    add_library(Ogg::ogg UNKNOWN IMPORTED)
    set_target_properties(Ogg::ogg PROPERTIES
      IMPORTED_LOCATION "${Ogg_LIBRARY}"
      INTERFACE_INCLUDE_DIRECTORIES "${Ogg_INCLUDE_DIR}")
  endif ()
endif ()
