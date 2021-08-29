cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

# ---- Project ----

# Note: update this to your new project's name and version
project(
  SspCpp
  VERSION 1.2.1
  DESCRIPTION "Sound Speed Profile Library"
  HOMEPAGE_URL https://github.com/DentonW/SspCpp
  LANGUAGES CXX
)

# ---- Set options ----
# (Needed before CPM calls)
#option(SSP_MATPLOTLIB_CPP_SUPPORT "Use matplotlib-cpp for plotting" OFF)
#option(SSP_COMPILE_EXAMPLES "Compile example programs" ON)
#option(SSP_COMPILE_TESTS "Compile test programs" OFF)

# ---- Add dependencies via CPM ----
# see https://github.com/TheLartians/CPM.cmake for more info

include(../cmake/CPM.cmake)

# PackageProject.cmake will be used to make our target installable
CPMAddPackage("gh:TheLartians/PackageProject.cmake@1.6.0")

CPMAddPackage(
  NAME fmt
  GIT_TAG 8.0.1
  GITHUB_REPOSITORY fmtlib/fmt
  OPTIONS "FMT_INSTALL YES" # create an installable target
)

CPMAddPackage("gh:HowardHinnant/date@3.0.1")

# Currently having trouble linking against this with CMake, so don't currently use it
#if (SSP_MATPLOTLIB_CPP_SUPPORT)
#    message(STATUS "Matplotlib++")
#    CPMAddPackage("gh:lava/matplotlib-cpp#ef0383f1315d32e0156335e10b82e90b334f6d9f")
#endif()


# ---- Add source files ----

set(headers
    ../include/SspCpp/Cast.h
    ../include/SspCpp/ProcessChecks.h
    ../include/SspCpp/SoundSpeed.h
	LatLong.h
    #../README.md
    StringUtilities.h
    TimeStruct.h
    Readers/Aoml.h
    Readers/Asvp.h
    Readers/SeaAndSun.h
    Readers/SeaBird.h
    Readers/Sonardyne.h
    Readers/Unb.h
)

set(sources
    LatLong.cpp
    Physical.cpp
    ProcessChecks.cpp
    SoundSpeed.cpp
    Readers/Aoml.cpp
    Readers/Asvp.cpp
    Readers/SeaAndSun.cpp
    Readers/SeaBird.cpp
    Readers/Sonardyne.cpp
    Readers/Unb.cpp
)

# Create Visual Studio filters to preserve the folder structure
if (CMAKE_GENERATOR MATCHES "Visual Studio")
    set(sourceheaders ${sources} ${headers})
    foreach(_source IN ITEMS ${sourceheaders})
        get_filename_component(_source_path "${_source}" PATH)
        string(REPLACE "${CMAKE_SOURCE_DIR}" "" _group_path "${_source_path}")
        string(REPLACE "../include/" "" _group_path "${_group_path}")  # Get rid of the ../include/ before the API headers
        string(REPLACE "/" "\\" _group_path "${_group_path}")
        source_group("${_group_path}" FILES "${_source}")
    endforeach()
endif()


# ---- Create library ----

# Note: for header-only libraries change all PUBLIC flags to INTERFACE and create an interface
# target: add_library(Greeter INTERFACE)
add_library(SspCpp ${headers} ${sources})

# Add precompiled header support
target_precompile_headers(SspCpp PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/pch.h>")

set_target_properties(SspCpp PROPERTIES CXX_STANDARD 17)

# being a cross-platform target, we enforce standards conformance on MSVC
target_compile_options(SspCpp PUBLIC "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/permissive->")

# Link dependencies
target_link_libraries(SspCpp PRIVATE fmt::fmt date::date)

target_include_directories(
  SspCpp PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/../include>
                 $<INSTALL_INTERFACE:include/${PROJECT_NAME}-${PROJECT_VERSION}>
)

# The API files are in the ../include folder instead of with the rest of the source (in src/)
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)


# ---- Create an installable target ----
# this allows users to install and find the library via `find_package()`.

# the location where the project's version header will be placed should match the project's regular
# header paths
string(TOLOWER ${PROJECT_NAME}/version.h VERSION_HEADER_LOCATION)

packageProject(
  NAME ${PROJECT_NAME}
  VERSION ${PROJECT_VERSION}
  NAMESPACE ${PROJECT_NAME}
  BINARY_DIR ${PROJECT_BINARY_DIR}
  INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include
  INCLUDE_DESTINATION include/${PROJECT_NAME}-${PROJECT_VERSION}
  VERSION_HEADER "${VERSION_HEADER_LOCATION}"
  COMPATIBILITY SameMajorVersion
  DEPENDENCIES "fmt 7.1.3;date 3.0.0"
)
