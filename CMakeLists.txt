cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

# ---- Project ----

# Note: update this to your new project's name and version
project(
  SspCpp-Main
  VERSION 1.6.3
  DESCRIPTION "Sound Speed Profile Library"
  HOMEPAGE_URL https://github.com/DentonW/SspCpp
  LANGUAGES CXX
)

# ---- Include guards ----

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
      "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
  )
endif()


# ---- Set options ----
# (Needed before CPM calls)
option(SSP_MATPLOTLIB_CPP_SUPPORT "Use matplotlib-cpp for plotting" OFF)
option(SSP_COMPILE_EXAMPLES "Compile example programs" ON)
option(SSP_COMPILE_TESTS "Compile test programs" OFF)


add_subdirectory(src/)
if (SSP_COMPILE_EXAMPLES)
    add_subdirectory(examples/)
endif()
if (SSP_COMPILE_TESTS)
    add_subdirectory(tests/)
endif()
