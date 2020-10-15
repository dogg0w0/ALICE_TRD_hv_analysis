# Find ROOT package
set(ROOT_DIR "${ROOTSYS}/cmake/")
find_package(ROOT 6.20 CONFIG)
if(ROOT_FOUND)
    message(STATUS "Found ROOT: ${ROOT_VERSION} at ${ROOT_DIR}")
    message(STATUS "\t\tOriginal flags: ${ROOT_CXX_FLAGS}")
    message(STATUS "\t\tOriginal definitions: ${ROOT_DEFINITIONS}")
    message(STATUS "\t\tOriginal exe flags: ${ROOT_EXE_LINKER_FLAGS}")
    # Make found targets globally available
    set_target_properties(
        ROOT::Physics
        ROOT::Core
        ROOT::Tree
        ROOT::Hist
        ROOT::Graf
        ROOT::Gpad
        PROPERTIES IMPORTED_GLOBAL TRUE
    )
    add_definitions(
    -Wall
    ${ROOT_CXX_FLAGS}
    )
else()
    message(STATUS "ROOT not found, did you source thisroot.sh yet?")
endif()