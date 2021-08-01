cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(SspTest LANGUAGES CXX)

# --- Import tools ----

#include(../cmake/tools.cmake)

# ---- Dependencies ----

include(../cmake/CPM.cmake)

#CPMAddPackage(NAME SspCpp SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/../src)

# ---- Create standalone executable ----

#file(GLOB sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)
set(sources test.cpp)

add_executable(SspTest ${sources})

set_target_properties(SspTest PROPERTIES CXX_STANDARD 17 OUTPUT_NAME "SspCpp")

target_link_libraries(SspTest SspCpp::SspCpp)
