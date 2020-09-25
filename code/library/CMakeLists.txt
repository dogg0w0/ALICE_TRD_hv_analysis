cmake_minimum_required(VERSION 3.16)
project(
    InternalLibraries
    VERSION 1.0
    DESCRIPTION "The internal root-func-libraries"
    LANGUAGES CXX
)

# Set path to library and LinkDef.h files
set(HEADER_LIST 
    "${CMAKE_CURRENT_SOURCE_DIR}/include/analysis_base.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/analysis_plots.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/analysis_hv.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/analysis_dates.hpp"
)
set(DEFLINK "${CMAKE_CURRENT_SOURCE_DIR}/include/DictLinkDef.h")

# Let ROOT generate a dictionary containing all the needed libraries
root_generate_dictionary(
    G__DictROOT
    "${HEADER_LIST}"
    LINKDEF "${DEFLINK}"
)

# Include G__DictROOT into our library and link all the targets to it
add_library(
    DictROOT 
    SHARED  G__DictROOT.cxx
)

target_sources(DictROOT
    PRIVATE "src/analysis_base.cpp"
    PRIVATE "src/analysis_plots.cpp"
    PRIVATE "src/analysis_hv.cpp"
    PRIVATE "src/analysis_dates.cpp"
    PUBLIC  "${HEADER_LIST}"
)

# Tell cmake where to look for our include path
target_include_directories( 
    DictROOT PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

# put the needed libraries into our dictionary for lookup
target_link_libraries(
    DictROOT  PUBLIC
    ROOT::Physics
    ROOT::Core
    ROOT::Tree
    ROOT::Hist
    ROOT::Graf
    ROOT::Gpad
)

# IDEs should put the headers in a nice place
source_group(
  TREE "${CMAKE_CURRENT_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${HEADER_LIST})

