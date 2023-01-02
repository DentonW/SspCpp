cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

# ---- Project ----

# Note: update this to your new project's name and version
project(
  SspCpp
  VERSION 1.6.0
  DESCRIPTION "SspCpp: Sound Speed Profile Library"
  HOMEPAGE_URL https://github.com/DentonW/SspCpp
  LANGUAGES CXX
)

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
          "BUILD_SHARED_LIBS FALSE"
)

CPMAddPackage("gh:HowardHinnant/date@3.0.1")

if (SSP_COMPILE_TESTS)
  CPMAddPackage(
    NAME googletest
    GITHUB_REPOSITORY google/googletest
    GIT_TAG release-1.8.1
    VERSION 1.8.1
    OPTIONS
        "INSTALL_GTEST OFF"
        "gtest_force_shared_crt ON"
  )
endif()

if (SSP_MATPLOTLIB_CPP_SUPPORT)
    # This project appears to be abandoned, so use my fork (has some updates)
    CPMAddPackage(
      NAME matplotlib_cpp
      GIT_TAG f59b63b52b400b76fde0dce9a899bd8fab5d1db5
      GITHUB_REPOSITORY DentonW/matplotlib-cpp
      OPTIONS "MATPLOTLIBCPP_BUILD_EXAMPLES OFF"
    )
    #if (matplotlib_cpp_ADDED)
    #    target_include_directories(matplotlib_cpp INTERFACE "${matplotlib_cpp_SOURCE_DIR}")
    #endif()
endif()


# ---- Add source files ----

set(headers
    ../include/SspCpp/Cast.h
    ../include/SspCpp/LatLong.h
    ../include/SspCpp/ProcessChecks.h
    ../include/SspCpp/SoundSpeed.h
    #../README.md
    StringUtilities.h
    TimeStruct.h
    Readers/Aoml.h
    Readers/Asvp.h
    Readers/Hypack.h
    Readers/Oceanscience.h
    Readers/SeaAndSun.h
    Readers/SeaBird.h
    Readers/Simple.h
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
    Readers/Hypack.cpp
    Readers/Oceanscience.cpp
    Readers/SeaAndSun.cpp
    Readers/SeaBird.cpp
    Readers/Simple.cpp
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

include(GenerateExportHeader)
generate_export_header(SspCpp)

# being a cross-platform target, we enforce standards conformance on MSVC
target_compile_options(SspCpp PUBLIC "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/permissive->")
target_compile_definitions(SspCpp PUBLIC SSPCPP_EXPORTS)

# Link dependencies
target_link_libraries(SspCpp PRIVATE date::date)
target_link_libraries(SspCpp PRIVATE fmt::fmt-header-only)

target_include_directories(
  SspCpp PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/../include>
                 $<INSTALL_INTERFACE:include/${PROJECT_NAME}-${PROJECT_VERSION}>
         PRIVATE ${PROJECT_SOURCE_DIR}
)

if (SSP_MATPLOTLIB_CPP_SUPPORT)
    message(STATUS "matplotlib-cpp support on")
    find_package (Python COMPONENTS Development)
    target_compile_definitions(SspCpp PRIVATE SSP_MATPLOTLIB_CPP_SUPPORT)
    target_link_libraries(SspCpp PRIVATE matplotlib_cpp)
    target_link_libraries(SspCpp PUBLIC Python::Python)
    target_include_directories(SspCpp PRIVATE ${matplotlib_cpp_SOURCE_DIR})
else()
    message(STATUS "matplotlib-cpp support off")
endif()

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
  INCLUDE_DIR ${PROJECT_SOURCE_DIR}/../include
  INCLUDE_DESTINATION include/${PROJECT_NAME}-${PROJECT_VERSION}
  VERSION_HEADER "${VERSION_HEADER_LOCATION}"
  COMPATIBILITY SameMajorVersion
  DEPENDENCIES "fmt 7.1.3"
  #DEPENDENCIES "fmt 7.1.3;date 3.0.0"
)

# ---- Make a library distributable package ----

include (CPack)
set(CPACK_NSIS_DEFINES "RequestExecutionLevel user")

# ---- Group external projects in Visual Studio ----
if (MSVC)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    if (TARGET fmt)
        set_target_properties(fmt PROPERTIES FOLDER Externals/gmock)
    endif()
    if (TARGET gmock)
        set_target_properties(gmock PROPERTIES FOLDER Externals/gmock)
    endif()
    if (TARGET gmock_main)
        set_target_properties(gmock_main PROPERTIES FOLDER Externals/gmock)
    endif()
    if (TARGET gtest)
        set_target_properties(gtest PROPERTIES FOLDER Externals/gmock)
    endif()
    if (TARGET gtest_main)
        set_target_properties(gtest_main PROPERTIES FOLDER Externals/gmock)
    endif()
endif()
