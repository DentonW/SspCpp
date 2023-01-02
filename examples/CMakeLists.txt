cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(SspExample LANGUAGES CXX)

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

set_target_properties(SspExample PROPERTIES CXX_STANDARD 17 OUTPUT_NAME "SspCpp")

target_link_libraries(SspExample SspCpp::SspCpp)

set(HYO2_LOC "${FETCHCONTENT_BASE_DIR}/hyo2_soundspeed-src/data/input/")
#file(WRITE hyo2_location.h
#     "#pragma once\n\nconst char* HYO2_LOC = \"${HYO2_LOC}\";\n"
#)
target_compile_definitions(SspExample PRIVATE HYO2_LOC="${HYO2_LOC}")
