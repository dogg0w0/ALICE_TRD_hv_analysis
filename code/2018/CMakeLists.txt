cmake_minimum_required(VERSION 3.16)
project(
    AlICE_TRD_HV_HL_ANALYSIS
    VERSION 1.0
    DESCRIPTION "analysis tools for hv in ALICE TRD"
    LANGUAGES CXX
)

# Require out-of-source builds
file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
endif()

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

  # Optionally set things like CMAKE_CXX_STANDARD, CMAKE_POSITION_INDEPENDENT_CODE here

  # Let's ensure -std=c++xx instead of -std=g++xx
  set(CMAKE_CXX_STANDARD 11)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
  set(CMAKE_PLATFORM_INDEPENDENT_CODE ON)

  # Let's nicely support folders in IDEs
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)

  # Docs only available if this is the main app
  #find_package(Doxygen)
  #if(Doxygen_FOUND)
  #  add_subdirectory(docs)
  #else()
  #  message(STATUS "Doxygen not found, not building docs")
  #endif()
endif()

# Add subdirectory to look fpr CMakeLists.txt files
add_subdirectory(external)
add_subdirectory(library)
add_subdirectory(apps)