cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

# ---- Project ----

# Note: update this to your new project's name and version
project(
  SspCpp-Main
  VERSION 1.6.0
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


# Add the cmake folder so the FindSphinx module is found
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# ---- Set options ----
# (Needed before CPM calls)
option(SSP_MATPLOTLIB_CPP_SUPPORT "Use matplotlib-cpp for plotting" OFF)
option(SSP_COMPILE_EXAMPLES "Compile example programs" ON)
option(SSP_COMPILE_TESTS "Compile test programs" OFF)
option(SSP_BUILD_DOCS "Build Doxygen+Sphinx documentation" OFF)


add_subdirectory(src/)
if (SSP_COMPILE_EXAMPLES)
    add_subdirectory(examples/)
endif()
if (SSP_COMPILE_TESTS)
    add_subdirectory(tests/)
endif()

if (SSP_BUILD_DOCS)
	add_subdirectory ("docs")
endif()
