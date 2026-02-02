cmake_minimum_required(VERSION 3.15)

# Simple CMake shim that reuses the existing GNU Make build system.
# It defines convenient CMake targets that invoke the repository's Makefile.

# Project declaration. We list C and ASM so CLion can properly index sources.
project(pokeemerald-expansion LANGUAGES C ASM)

# Export compile commands for better IDE integration (CLion picks this up automatically).
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Default to a sensible C standard for IDE features. The real build uses the Makefile.
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Locate a GNU Make executable explicitly. Using CMAKE_MAKE_PROGRAM would
# point to the CMake generator's build tool (e.g., Ninja) when the Ninja
# generator is selected, which is not what we want. We need to drive the
# repository's GNU Makefile regardless of the CMake generator.
find_program(GNU_MAKE_EXE NAMES gmake make mingw32-make)
if(NOT GNU_MAKE_EXE)
    message(FATAL_ERROR "GNU Make not found. Please install 'make' and ensure it is in PATH.")
endif()

# Helper to create a make-backed target that always uses GNU Make
function(add_make_target tgt)
    set(options ALL)
    set(oneValueArgs GOAL)
    set(multiValueArgs ARGS)
    cmake_parse_arguments(MK "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(NOT MK_GOAL)
        message(FATAL_ERROR "add_make_target requires GOAL=<make-goal>")
    endif()

    # Build the command: make -C <source_dir> <goal> <args...>
    set(cmd ${GNU_MAKE_EXE} -C ${CMAKE_SOURCE_DIR} ${MK_GOAL} ${MK_ARGS})

    if(MK_ALL)
        add_custom_target(${tgt} ALL
                COMMAND ${cmd}
                USES_TERMINAL
                VERBATIM
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                COMMENT "Invoking Make: ${MK_GOAL} ${MK_ARGS}")
    else()
        add_custom_target(${tgt}
                COMMAND ${cmd}
                USES_TERMINAL
                VERBATIM
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                COMMENT "Invoking Make: ${MK_GOAL} ${MK_ARGS}")
    endif()
endfunction()

# Default build (ALL) will invoke the Makefile's `all` goal
# We can't name a CMake target "all" (reserved), so we create a default
# target that is part of ALL and calls `make all` under the hood.
add_make_target(make_all ALL GOAL all)

# Keep a convenience target to build just the ROM via `make rom`
add_make_target(rom GOAL rom)

# Common convenience targets mirroring the Makefile goals
add_make_target(release GOAL release)
add_make_target(debug   GOAL debug)
add_make_target(check   GOAL check)
add_make_target(compare GOAL compare)
add_make_target(tools   GOAL tools)
add_make_target(make-clean GOAL clean)

# Alternate game versions
# These ensure the variant is selected and then the ROM is built.
add_make_target(firered GOAL firered ARGS rom)
add_make_target(leafgreen GOAL leafgreen ARGS rom)

# Print a message guiding users
message(STATUS "Configured CMake shim. Build targets available:")
message(STATUS "  - make_all (default -> runs 'make all')")
message(STATUS "  - release, debug, check, compare, tools, make-clean")
message(STATUS "  - firered, leafgreen (build variant ROMs)")

#
# IDE integration (no-build) target
# ----------------------------------
# Expose the project's sources to CLion without trying to build them with CMake.
# The actual build is driven by GNU Make targets above. This section only improves
# code navigation, search, and refactoring inside the IDE.

# Collect common source files for indexing. This is non-exhaustive but covers the
# usual locations. Add more patterns if needed.
file(GLOB_RECURSE POKEMERALD_C_SOURCES
        CONFIGURE_DEPENDS
        ${CMAKE_SOURCE_DIR}/src/*.c
        ${CMAKE_SOURCE_DIR}/src/*/*.c
)
file(GLOB_RECURSE POKEMERALD_HEADERS
        CONFIGURE_DEPENDS
        ${CMAKE_SOURCE_DIR}/include/*.h
        ${CMAKE_SOURCE_DIR}/include/*/*.h
)
file(GLOB_RECURSE POKEMERALD_ASM
        CONFIGURE_DEPENDS
        ${CMAKE_SOURCE_DIR}/src/*.s
        ${CMAKE_SOURCE_DIR}/asm/*.s
        ${CMAKE_SOURCE_DIR}/asm/*/*.s
)

set(POKEMERALD_ALL_SOURCES
        ${POKEMERALD_C_SOURCES}
        ${POKEMERALD_HEADERS}
        ${POKEMERALD_ASM}
)

# An IDE-only target that lists sources but does not build anything.
add_custom_target(ide SOURCES ${POKEMERALD_ALL_SOURCES})

# Provide basic include paths for code completion.
add_library(pokeemerald_headers INTERFACE)
target_include_directories(pokeemerald_headers INTERFACE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src
)

add_dependencies(ide pokeemerald_headers)
