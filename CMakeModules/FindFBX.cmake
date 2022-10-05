#[=======================================================================[.rst:
FindFBX
----------

Finds the FBX SDK.

Imported Targets
^^^^^^^^^^^^^^^^

This module provides the following imported targets, if found:

``FBX::FBX``
  FBX SDK release version

``FBX::FBX_d``
  FBX SDK debug version

Result Variables
^^^^^^^^^^^^^^^^

This will define the following variables:

``FBX_INCLUDE_DIRS``
  List of include directories when using FBX.
``FBX_LIBRARIES``
  List of libraries when using FBX.
``FBX_FOUND``
  True if FBX found.

Cache variables
^^^^^^^^^^^^^^^

The following cache variables may also be set:

``FBX_INCLUDE_DIR``
  The directory containing ``fbxsdk.h``.
``FBX_LIBRARY``
  The path to the FBX library.

Hints
^^^^^

A user may set ``FBX_ROOT`` to a FBX SDK installation root to tell this module where to look.

#]=======================================================================]

IF(APPLE)
  if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    # using regular Clang or AppleClang
    SET(FBX_LIBDIR "clang")
  else()
    SET(FBX_LIBDIR "gcc4/ub")
  endif()
ELSEIF(CMAKE_COMPILER_IS_GNUCXX)
    SET(FBX_LIBDIR "gcc4")
    SET(FBX_LIBDIR "vs2012")
ELSEIF(MSVC_VERSION EQUAL 1800)
    SET(FBX_LIBDIR "vs2013")
ELSEIF(MSVC_VERSION EQUAL 1900)
    SET(FBX_LIBDIR "vs2015")
ELSEIF(MSVC_VERSION GREATER_EQUAL 1910 AND MSVC_VERSION LESS 1920)
    SET(FBX_LIBDIR "vs2017")
ELSEIF(MSVC_VERSION GREATER_EQUAL 1920 AND MSVC_VERSION LESS 1930)
    SET(FBX_LIBDIR "vs2019")
ENDIF()

IF(APPLE)
    # do nothing
ELSEIF(CMAKE_CL_64)
    SET(FBX_LIBDIR ${FBX_LIBDIR}/x64)
ELSEIF(CMAKE_SIZEOF_VOID_P EQUAL 8)
    SET(FBX_LIBDIR ${FBX_LIBDIR}/x64)
ELSE()
    SET(FBX_LIBDIR ${FBX_LIBDIR}/x86)
ENDIF()

IF(APPLE)
    SET(FBX_LIBNAME "libfbxsdk.a")
ELSEIF(CMAKE_COMPILER_IS_GNUCXX)
    SET(FBX_LIBNAME "fbxsdk")
ELSE()
    OPTION(FBX_SHARED OFF)
    IF(FBX_SHARED)
        SET(FBX_LIBNAME "libfbxsdk")
    ELSE()
        SET(FBX_LIBNAME "libfbxsdk-md")
        IF(WIN32)
            SET(FBX_XML2_LIBNAME "libxml2-md")
            SET(FBX_ZLIB_LIBNAME "zlib-md")
        ENDIF()
    ENDIF()
ENDIF()

SET(FBX_LIBNAME_DEBUG ${FBX_LIBNAME}d)

SET(FBX_SEARCH_PATHS
    $ENV{FBX_ROOT}
    "$ENV{ProgramW6432}/Autodesk/FBX/FBX SDK/2020.3.1"
    "$ENV{PROGRAMFILES}/Autodesk/FBX/FBX SDK/2020.3.1"
    "/Applications/Autodesk/FBX SDK/2020.3.1"
    "$ENV{ProgramW6432}/Autodesk/FBX/FBX SDK/2020.2.1"
    "$ENV{PROGRAMFILES}/Autodesk/FBX/FBX SDK/2020.2.1"
    "/Applications/Autodesk/FBX SDK/2020.2.1"
    "$ENV{ProgramW6432}/Autodesk/FBX/FBX SDK/2020.2"
    "$ENV{PROGRAMFILES}/Autodesk/FBX/FBX SDK/2020.2"
    "/Applications/Autodesk/FBX SDK/2020.2"
)

# search for headers & debug/release libraries
FIND_PATH(FBX_INCLUDE_DIR "fbxsdk.h"
    PATHS ${FBX_SEARCH_PATHS}
    PATH_SUFFIXES "include")

FIND_LIBRARY(FBX_LIBRARY ${FBX_LIBNAME}
    PATHS ${FBX_SEARCH_PATHS}
    PATH_SUFFIXES "lib/${FBX_LIBDIR}/release" "lib/${FBX_LIBDIR}")

#Once one of the calls succeeds the result variable will be set and stored in the cache so that no call will search again.

#no debug d suffix, search in debug folder only
FIND_LIBRARY(FBX_LIBRARY_DEBUG ${FBX_LIBNAME}
    PATHS ${FBX_SEARCH_PATHS}
    PATH_SUFFIXES "lib/${FBX_LIBDIR}/debug")

FIND_LIBRARY(FBX_LIBRARY_DEBUG ${FBX_LIBNAME_DEBUG}
    PATHS ${FBX_SEARCH_PATHS}
    PATH_SUFFIXES "lib/${FBX_LIBDIR}")

IF(WIN32)
    FIND_LIBRARY( FBX_XML2_LIBRARY ${FBX_XML2_LIBNAME}
        PATHS ${FBX_SEARCH_PATHS}
        PATH_SUFFIXES "lib/${FBX_LIBDIR}/release" "lib/${FBX_LIBDIR}")
    FIND_LIBRARY( FBX_ZLIB_LIBRARY ${FBX_ZLIB_LIBNAME}
        PATHS ${FBX_SEARCH_PATHS}
        PATH_SUFFIXES "lib/${FBX_LIBDIR}/release" "lib/${FBX_LIBDIR}")
    FIND_LIBRARY( FBX_XML2_LIBRARY_DEBUG ${FBX_XML2_LIBNAME}
        PATHS ${FBX_SEARCH_PATHS}
        PATH_SUFFIXES "lib/${FBX_LIBDIR}/debug")
    FIND_LIBRARY( FBX_ZLIB_LIBRARY_DEBUG ${FBX_ZLIB_LIBNAME}
        PATHS ${FBX_SEARCH_PATHS}
        PATH_SUFFIXES "lib/${FBX_LIBDIR}/debug")
ENDIF()

find_package_handle_standard_args(FBX REQUIRED_VARS FBX_LIBRARY FBX_LIBRARY_DEBUG FBX_INCLUDE_DIR)

if (FBX_FOUND)
  set(FBX_LIBRARIES "${FBX_LIBRARY}")
  set(FBX_LIBRARIES_DEBUG "${FBX_LIBRARY_DEBUG}")
  set(FBX_INCLUDE_DIRS "${FBX_INCLUDE_DIR}")

  if (NOT TARGET FBX::FBX)
    add_library(FBX::FBX UNKNOWN IMPORTED)
    set_target_properties(FBX::FBX PROPERTIES
      IMPORTED_LOCATION "${FBX_LIBRARY}"
      INTERFACE_INCLUDE_DIRECTORIES "${FBX_INCLUDE_DIR}")
  endif ()

  if (NOT TARGET FBX::FBX_d)
    add_library(FBX::FBX_d UNKNOWN IMPORTED)
    set_target_properties(FBX::FBX_d PROPERTIES
      IMPORTED_LOCATION "${FBX_LIBRARY_DEBUG}"
      INTERFACE_INCLUDE_DIRECTORIES "${FBX_INCLUDE_DIR}")
  endif ()

endif ()