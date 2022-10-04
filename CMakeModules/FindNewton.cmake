#[=======================================================================[.rst:
FindNewton
----------

Finds the Newton library.

Imported Targets
^^^^^^^^^^^^^^^^

This module provides the following imported targets, if found:

``Newton::Newton``
  The Newton library

Result Variables
^^^^^^^^^^^^^^^^

This will define the following variables:

``Newton_INCLUDE_DIRS``
  List of include directories when using Newton.
``Newton_LIBRARIES``
  List of libraries when using Newton.
``Newton_FOUND``
  True if Newton and requested components found.
``Newton_FOUND``
  True if Newton found.

Cache variables
^^^^^^^^^^^^^^^

The following cache variables may also be set:

``Newton_INCLUDE_DIR``
  The directory containing ``Newton.h``.
``Newton_LIBRARY``
  The path to the Newton library.

Hints
^^^^^

A user may set ``Newton_ROOT`` to a Newton installation root to tell this module where to look.

#]=======================================================================]

find_path (Newton_INCLUDE_DIR
  NAMES
    Newton.h
  HINTS
    ${Newton_ROOT}
  )

find_library (Newton_LIBRARY
  NAMES
    newton
    Newton
  HINTS
    ${Newton_ROOT}
  )

include (FindPackageHandleStandardArgs)
find_package_handle_standard_args (Newton REQUIRED_VARS Newton_LIBRARY Newton_INCLUDE_DIR)

if (Newton_FOUND)
  set (Newton_LIBRARIES "${Newton_LIBRARY}")
  set (Newton_INCLUDE_DIRS "${Newton_INCLUDE_DIR}")
  if (NOT TARGET Newton::Newton)
    add_library (Newton::Newton UNKNOWN IMPORTED)
    set_target_properties (Newton::Newton PROPERTIES
      IMPORTED_LOCATION "${Newton_LIBRARY}"
      INTERFACE_INCLUDE_DIRECTORIES ${Newton_INCLUDE_DIR}
    )
  endif ()
endif ()

mark_as_advanced (Newton_INCLUDE_DIR)
mark_as_advanced (Newton_LIBRARY)