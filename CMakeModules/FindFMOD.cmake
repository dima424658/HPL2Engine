#[=======================================================================[.rst:
FindFMOD
----------

Finds the FMOD library.

Imported Targets
^^^^^^^^^^^^^^^^

This module provides the following imported targets, if found:

``FMOD::FMOD``
  The FMOD library

Result Variables
^^^^^^^^^^^^^^^^

This will define the following variables:

``FMOD_INCLUDE_DIRS``
  List of include directories when using FMOD.
``FMOD_LIBRARIES``
  List of libraries when using FMOD.
``FMOD_FOUND``
  True if FMOD and requested components found.
``FMOD_FOUND``
  True if FMOD found.

Cache variables
^^^^^^^^^^^^^^^

The following cache variables may also be set:

``FMOD_INCLUDE_DIR``
  The directory containing ``FMOD/FMOD.h``.
``FMOD_LIBRARY``
  The path to the FMOD library.

Hints
^^^^^

A user may set ``FMOD_ROOT`` to a FMOD installation root to tell this module where to look.

#]=======================================================================]

find_path (FMOD_INCLUDE_DIR
  NAMES
    fmod.h
  HINTS
    ${FMOD_ROOT}
  PATH_SUFFIXES
    inc
    core/inc
    api/core/inc
    )

find_library (FMOD_LIBRARY
  NAMES
    fmod
  HINTS
    ${FMOD_ROOT}
  PATH_SUFFIXES
    lib
    core/lib
    api/core/lib
  )

include (FindPackageHandleStandardArgs)
find_package_handle_standard_args (FMOD REQUIRED_VARS FMOD_LIBRARY FMOD_INCLUDE_DIR)

if (FMOD_FOUND)
  set (FMOD_LIBRARIES "${FMOD_LIBRARY}")
  set (FMOD_INCLUDE_DIRS "${FMOD_INCLUDE_DIR}")
  if (NOT TARGET FMOD::FMOD)
    add_library (FMOD::FMOD UNKNOWN IMPORTED)
    set_target_properties (FMOD::FMOD PROPERTIES
      IMPORTED_LOCATION "${FMOD_LIBRARY}"
      INTERFACE_INCLUDE_DIRECTORIES ${FMOD_INCLUDE_DIR})
  endif ()
endif ()

mark_as_advanced (FMOD_INCLUDE_DIR)
mark_as_advanced (FMOD_LIBRARY)