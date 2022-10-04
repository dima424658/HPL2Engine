#[=======================================================================[.rst:
FindAngelScript
----------

Finds the AngelScript library.

Imported Targets
^^^^^^^^^^^^^^^^

This module provides the following imported targets, if found:

``AngelScript::AngelScript``
  The AngelScript library

Result Variables
^^^^^^^^^^^^^^^^

This will define the following variables:

``AngelScript_INCLUDE_DIRS``
  List of include directories when using AngelScript.
``AngelScript_LIBRARIES``
  List of libraries when using AngelScript.
``AngelScript_FOUND``
  True if AngelScript and requested components found.
``AngelScript_FOUND``
  True if AngelScript found.

Cache variables
^^^^^^^^^^^^^^^

The following cache variables may also be set:

``AngelScript_INCLUDE_DIR``
  The directory containing ``angelscript.h``.
``AngelScript_LIBRARY``
  The path to the AngelScript library.

Hints
^^^^^

A user may set ``AngelScript_ROOT`` to a AngelScript installation root to tell this module where to look.

#]=======================================================================]

find_path (AngelScript_INCLUDE_DIR
  NAMES
    angelscript.h
  HINTS
    ${AngelScript_ROOT}
  )

find_library (AngelScript_LIBRARY
  NAMES
    angelscript
  HINTS
    ${AngelScript_ROOT}
  )

include (FindPackageHandleStandardArgs)
find_package_handle_standard_args (AngelScript REQUIRED_VARS AngelScript_LIBRARY AngelScript_INCLUDE_DIR)

if (AngelScript_FOUND)
  set (AngelScript_LIBRARIES "${AngelScript_LIBRARY}")
  set (AngelScript_INCLUDE_DIRS "${AngelScript_INCLUDE_DIR}")
  if (NOT TARGET AngelScript::AngelScript)
    add_library (AngelScript::AngelScript UNKNOWN IMPORTED)
    set_target_properties (AngelScript::AngelScript PROPERTIES
      IMPORTED_LOCATION "${AngelScript_LIBRARY}"
      INTERFACE_INCLUDE_DIRECTORIES ${AngelScript_INCLUDE_DIR}
    )
  endif ()
endif ()

mark_as_advanced (AngelScript_INCLUDE_DIR)
mark_as_advanced (AngelScript_LIBRARY)