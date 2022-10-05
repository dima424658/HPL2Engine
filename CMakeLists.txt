cmake_minimum_required (VERSION 3.21)
project(HPL2)

set(HPL2_VERSION_MAJOR 2)
set(HPL2_VERSION_MINOR 0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options(-fdiagnostics-color=always)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_compile_options(-fcolor-diagnostics)
endif()

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

if(APPLE)
    list(APPEND apple_sources sources/impl/PlatformMacOSX.mm)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework Foundation -framework AppKit")
    list(APPEND impl_sources ${apple_sources})
endif()

LIST(APPEND impl_sources
    sources/impl/PlatformUnix.cpp
    sources/impl/PlatformSDL.cpp
)

# Setup buildID source

if(APPLE)
    SET(VERSION_SUFFIX MacOSX)
elseif(UNIX)
    SET(VERSION_SUFFIX Linux)
elseif(WIN32)
    SET(VERSION_SUFFIX Win32)
else()
    message(FATAL_ERROR "Unknown target system.")
endif()

SET(version_source sources/BuildID_HPL2_0_${VERSION_SUFFIX}.cpp)

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

# setup HPL2 compile target
add_library(HPL2 STATIC
    ${common_sources}
    ${version_sources}
    ${impl_sources}
)

# OpenGL
find_package(OpenGL REQUIRED)

# GLEW
find_package(GLEW REQUIRED)

# Theora
find_package(Theora REQUIRED)

# DevIL
find_package(DevIL REQUIRED)

# Zlib
find_package(ZLIB REQUIRED)

# POSIX Threads
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

# Find the XML processing library
find_package(LibXml2 REQUIRED)

# Zlib
find_package(FBX REQUIRED)

# AngelScript
set(AngelScript_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/angelscript)
find_package(AngelScript REQUIRED)

# AngelScript
set(Newton_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/newton)
find_package(Newton REQUIRED)

# Find SDL2
find_package(SDL2 REQUIRED)

# Binreloc
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/binreloc)

# OALWrapper
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/OALWrapper)

target_link_libraries(HPL2
    OALWrapper
    OpenGL::GL
    OpenGL::GLU 
    GLEW::GLEW
    Theora::Theora
    DevIL::IL
    ZLIB::ZLIB
    LibXml2::LibXml2
    SDL2::SDL2
    Threads::Threads
    Newton::Newton
    AngelScript::AngelScript
    binreloc
    FBX::FBX
    iconv
    ${CMAKE_DL_LIBS}
)

target_include_directories(HPL2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

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