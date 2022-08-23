cmake_minimum_required (VERSION 3.21)
project(HPL2)

set(HPL2_VERSION_MAJOR 2)
set(HPL2_VERSION_MINOR 0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options(-fdiagnostics-color=always)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_compile_options(-fcolor-diagnostics)
endif()

set(HPL2_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(DEP_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../dependencies/include")
set(DEP_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../dependencies/lib/${BIN_LIBROOT}")

SET(PRIVATE_LIB_SUFFIX "lib/${BIN_LIBROOT}")
SET(PRIVATE_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../dependencies)

add_subdirectory(../dependencies/OALWrapper OALWrapper)

add_definitions(
    -DUSE_OALWRAPPER
)

include_directories(
    include
    #Important that this is before dependencies so the correct OpenAL headers are used
    ${OALWRAPPER_INCLUDE_DIR}
    ../dependencies/include
)

add_library(binreloc STATIC ../dependencies/sources/binreloc/binreloc.c)
set_target_properties(binreloc PROPERTIES COMPILE_DEFINITIONS "ENABLE_BINRELOC")

include_directories(
    ../dependencies/sources/binreloc/
)

# common shared source
file(GLOB common_sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    sources/ai/*
    sources/engine/*
    sources/generate/*
    sources/graphics/*
    sources/gui/*
    sources/haptic/*
    sources/input/*
    sources/math/*
    sources/physics/*
    sources/resources/*
    sources/scene/*
    sources/sound/*
    sources/system/*

    include/ai/*
    include/engine/*
    include/generate/*
    include/graphics/*
    include/gui/*
    include/haptic/*
    include/input/*
    include/math/*
    include/physics/*
    include/resources/*
    include/scene/*
    include/sound/*
    include/system/*
    include/*.*
)

list(APPEND common_sources include/BuildID_HPL2_0.h)
list(REMOVE_DUPLICATES common_sources)

#setup various IMPL sources
file(GLOB impl_sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    include/impl/tinyXML/*
    include/impl/*.*

    # tinyXML
    sources/impl/tinyXml/*
    sources/impl/XmlDocumentTiny.cpp
    # scripting
    sources/impl/SqScript.cpp
    sources/impl/scriptarray.cpp
    sources/impl/scripthelper.cpp
    sources/impl/scriptstdstring.cpp
    sources/impl/scriptstdstring_utils.cpp
    # Image loading
    sources/impl/BitmapLoader*
    # theora video
    sources/impl/VideoStreamTheora.cpp
    # Newton
    sources/impl/*Newton.cpp
    # GL
    sources/impl/FrameBufferGL.cpp
    sources/impl/GLSL*
    sources/impl/OcclusionQueryOGL.cpp
    sources/impl/VertexBufferOGL_Array.cpp
    sources/impl/VertexBufferOGL_VBO.cpp
    sources/impl/VertexBufferOpenGL.cpp
    # SDL
    sources/impl/GamepadSDL.cpp
    sources/impl/KeyboardSDL.cpp
    sources/impl/MouseSDL.cpp
    sources/impl/MutexSDL.cpp
    sources/impl/ThreadSDL.cpp
    sources/impl/TimerSDL.cpp
    sources/impl/LowLevelGraphicsSDL.cpp
    sources/impl/LowLevelInputSDL.cpp
    sources/impl/LowLevelResourcesSDL.cpp
    sources/impl/LowLevelSystemSDL.cpp
    sources/impl/SDLEngineSetup.cpp
    sources/impl/SDLFontData.cpp
    sources/impl/SDLTexture.cpp
    # OpenAL
    sources/impl/LowLevelSoundOpenAL.cpp
    sources/impl/OpenAL*
    # mesh loader
    sources/impl/MeshLoaderCollada.cpp
    sources/impl/MeshLoaderColladaHelpers.cpp
    sources/impl/MeshLoaderColladaLoader.cpp
    sources/impl/MeshLoaderMSH.cpp
    sources/impl/MeshLoaderFBX.cpp
)

LIST(APPEND impl_sources
    sources/impl/PlatformUnix.cpp
    sources/impl/PlatformSDL.cpp
)

# Setup buildID source

SET(VERSION_SUFFIX Linux)

SET(version_source
    sources/BuildID_HPL2_0_${VERSION_SUFFIX}.cpp
)

set(BUILDCOUNTER_BIN "${CMAKE_CURRENT_SOURCE_DIR}/buildcounter.pl")
set(BUILDCOUNTER_BIN ${BUILDCOUNTER_BIN})

add_custom_command(
    OUTPUT
        sources/BuildID_HPL2_0_${VERSION_SUFFIX}.cpp
        include/BuildID_HPL2_0.h
    COMMAND
        ${BUILDCOUNTER_BIN}
    ARGS
        HPL2_0 ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/sources
)

# setup source groups
source_group("Implementation Specific" REGULAR_EXPRESSION "\\/impl\\/.+")
source_group("TinyXML" REGULAR_EXPRESSION "\\/impl\\/tiny.+")
source_group("SDL Specific" REGULAR_EXPRESSION "\\/impl\\/.*SDL.+")
source_group("OpenGL Specific" REGULAR_EXPRESSION "\\/impl\\/.*GL.+")
source_group("HaptX Specific" REGULAR_EXPRESSION "\\/impl\\/.+HaptX.+")
source_group("Newton Specific" REGULAR_EXPRESSION "\\/impl\\/.+Newton.+")
source_group("Mesh Loaders" REGULAR_EXPRESSION "\\/impl\\/Mesh.+")
source_group("OpenAL Specific" REGULAR_EXPRESSION "\\/impl\\/.*OpenAL.+")
source_group("Win32" REGULAR_EXPRESSION "\\/impl\\/.+Win32.+")
source_group("Platform" REGULAR_EXPRESSION "\\/impl\\/Platform.+")


# setup libs

add_library(Newton STATIC IMPORTED)
set_target_properties(Newton PROPERTIES IMPORTED_LOCATION ${DEP_LIB_DIR}/libNewton.a)

add_library(angelscript STATIC IMPORTED)
set_target_properties(angelscript PROPERTIES IMPORTED_LOCATION ${DEP_LIB_DIR}/libangelscript.a)

add_library(FBX STATIC IMPORTED)
set_target_properties(FBX PROPERTIES IMPORTED_LOCATION ${DEP_LIB_DIR}/libfbxsdk.a)

# setup HPL2 compile target

add_library(HPL2 STATIC
    ${common_sources}
    ${version_sources}
    ${impl_sources}
)

# OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(HPL2 OpenGL::OpenGL OpenGL::GLU)

# GLEW
find_package(GLEW REQUIRED)
target_link_libraries(HPL2 GLEW::GLEW)

# Theora
find_package(Theora REQUIRED)
target_link_libraries(HPL2 Theora::Theora)

# DevIL
find_package(DevIL REQUIRED)
target_link_libraries(HPL2 DevIL::IL)

find_package(ZLIB REQUIRED)
target_link_libraries(HPL2 ZLIB::ZLIB)

# POSIX Threads
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)
target_link_libraries(HPL2 Threads::Threads)

# Find the XML processing library
find_package(LibXml2 REQUIRED)
target_link_libraries(HPL2 LibXml2::LibXml2)

# Find SDL2
find_package(SDL2 REQUIRED)
target_link_libraries(HPL2 SDL2::SDL2)

target_link_libraries(HPL2
    OALWrapper # also pulls in ogg,vorbis,SDL
    Newton
    angelscript
    binreloc
    FBX
    ${CMAKE_DL_LIBS}
)

FUNCTION(AddTestTarget target_name)
    add_executable(${target_name} EXCLUDE_FROM_ALL
        ${ARGN}
    )
    target_link_libraries(${target_name} HPL2)
    set_target_properties(${target_name} PROPERTIES
        XCODE_ATTRIBUTE_LD_RUNPATH_SEARCH_PATHS "${DEP_LIB_DIR}"
        INSTALL_RPATH "${DEP_LIB_DIR}"
    )
ENDFUNCTION()

add_subdirectory(tools/editors editors)
add_subdirectory(tools/mshconverter mshconverter)