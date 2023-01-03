cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

#project(SspExample LANGUAGES CXX)

# --- Import tools ----

#include(../cmake/tools.cmake)

# ---- Dependencies ----

#include(../cmake/CPM.cmake)

#CPMAddPackage(NAME SspCpp SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/../src)
# This doesn't seem to work on anything other than my computer...
#CPMAddPackage("gh:hydroffice/hyo2_soundspeed#2021.1.7")
#message(STATUS "hyo2: ${FETCHCONTENT_SOURCE_DIR_HYO2_SOUNDSPEED}")
include(FetchContent)
FetchContent_Declare(
  hyo2_soundspeed
  #URL      https://github.com/hydroffice/hyo2_soundspeed/archive/refs/tags/v.2021.2.2.zip
  URL      https://github.com/hydroffice/hyo2_soundspeed/archive/refs/tags/v.2022.2.13.zip
  #URL_HASH MD5=1b5c4c8c5c1893ed1cde475852cace51
)
FetchContent_MakeAvailable(hyo2_soundspeed)

# ---- Create standalone executable ----

#file(GLOB sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)
set(sources Main.cpp)

add_executable(SspExample ${sources})

set_target_properties(SspExample PROPERTIES	CXX_STANDARD 17)
# If not being used as a library by another project, put the shared library and example executable in the same folder.
if (PROJECT_IS_TOP_LEVEL)
    set_target_properties(SspExample PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin"
    )
endif()


target_link_libraries(SspExample PUBLIC SspCpp::SspCpp)

set(HYO2_LOC "${FETCHCONTENT_BASE_DIR}/hyo2_soundspeed-src/data/input/")
#file(WRITE hyo2_location.h
#     "#pragma once\n\nconst char* HYO2_LOC = \"${HYO2_LOC}\";\n"
#)
target_compile_definitions(SspExample PRIVATE HYO2_LOC="${HYO2_LOC}")

#target_include_directories(
#  SspExample PRIVATE ${PROJECT_BINARY_DIR} ${PROJECT_BINARY_DIR}/src
#)
