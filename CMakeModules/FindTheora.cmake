#[=======================================================================[.rst:
FindTheora
----------

Finds the Theora library.

Imported Targets
^^^^^^^^^^^^^^^^

This module provides the following imported targets, if found:

``Theora::Theora``
  The Theora library

Result Variables
^^^^^^^^^^^^^^^^

This will define the following variables:

``Theora_INCLUDE_DIRS``
  List of include directories when using Theora.
``Theora_LIBRARIES``
  List of libraries when using Theora.
``Theora_FOUND``
  True if Theora and requested components found.
``Theora_FOUND``
  True if Theora found.

Cache variables
^^^^^^^^^^^^^^^

The following cache variables may also be set:

``Theora_INCLUDE_DIR``
  The directory containing ``Theora/Theora.h``.
``Theora_LIBRARY``
  The path to the Theora library.

Hints
^^^^^

A user may set ``Theora_ROOT`` to a Theora installation root to tell this module where to look.

#]=======================================================================]

find_package (Ogg QUIET)

find_path (Theora_INCLUDE_DIR
  NAMES
    theora/theora.h
  HINTS
    ${Theora_ROOT}
  )

find_library (Theora_LIBRARY
  NAMES
    theora
  HINTS
    ${Theora_ROOT}
  )

include (FindPackageHandleStandardArgs)
find_package_handle_standard_args (Theora REQUIRED_VARS Ogg_FOUND Theora_LIBRARY Theora_INCLUDE_DIR)

if (Theora_FOUND)
  set (Theora_LIBRARIES "${Theora_LIBRARY}")
  set (Theora_INCLUDE_DIRS "${Theora_INCLUDE_DIR}")
  if (NOT TARGET Theora::Theora)
    add_library (Theora::Theora UNKNOWN IMPORTED)
    set_target_properties (Theora::Theora PROPERTIES
      IMPORTED_LOCATION "${Theora_LIBRARY}"
      INTERFACE_INCLUDE_DIRECTORIES ${Theora_INCLUDE_DIR}
      INTERFACE_LINK_LIBRARIES Ogg::ogg
    )
  endif ()
endif ()

mark_as_advanced (Theora_INCLUDE_DIR)
mark_as_advanced (Theora_LIBRARY)