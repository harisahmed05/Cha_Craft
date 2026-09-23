# SPDX-License-Identifier: MIT
#
# Cha Craft - The Tea Lounge
# Build with: cmake -S . -B build && cmake --build build -j
#
# raylib is searched for in this order:
#   1. find_package(raylib CONFIG)         - Homebrew, vcpkg, raylib >= 4.0
#   2. pkg_check_modules(RAYLIB raylib)    - many Linux distros
#   3. find_library / find_path           - source-built installs
#
# To force a build with no system raylib using FetchContent, pass
# -DCHACRAFT_FETCH_RAYLIB=ON (downloads raylib from GitHub at configure time).

cmake_minimum_required(VERSION 3.16)
project(ChaCraft
    VERSION 1.0.0
    DESCRIPTION "A cozy barista simulation built with C++17 and raylib."
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

option(CHACRAFT_FETCH_RAYLIB "Download and build raylib from source via FetchContent" OFF)

# ---------------------------------------------------------------------------
# Locate raylib
# ---------------------------------------------------------------------------
set(RAYLIB_FOUND_METHOD "")

find_package(raylib QUIET CONFIG)
if(raylib_FOUND)
    message(STATUS "Found raylib via find_package (raylib ${raylib_VERSION})")
    set(RAYLIB_FOUND_METHOD "cmake")
else()
    include(FindPkgConfig OPTIONAL RESULT_VARIABLE PKG_CONFIG_OK)
    if(PKG_CONFIG_OK)
        pkg_check_modules(RAYLIB QUIET IMPORTED_TARGET raylib)
    endif()
    if(RAYLIB_FOUND)
        message(STATUS "Found raylib via pkg-config")
        set(RAYLIB_FOUND_METHOD "pkg-config")
    else()
        find_path(RAYLIB_INCLUDE_DIR
            NAMES raylib.h
            PATHS /usr/include /usr/local/include /opt/homebrew/include
            DOC "raylib header directory")
        find_library(RAYLIB_LIBRARY
            NAMES raylib
            PATHS /usr/lib /usr/local/lib /usr/lib/x86_64-linux-gnu /opt/homebrew/lib
            DOC "raylib library")
        if(RAYLIB_INCLUDE_DIR AND RAYLIB_LIBRARY)
            message(STATUS "Found raylib at ${RAYLIB_LIBRARY}")
            set(RAYLIB_FOUND_METHOD "manual")
        else()
            if(CHACRAFT_FETCH_RAYLIB)
                message(STATUS "raylib not found locally - fetching via FetchContent")
                include(FetchContent)
                set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
                set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)
                set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
                FetchContent_Declare(
                    raylib
                    GIT_REPOSITORY https://github.com/raysan5/raylib.git
                    GIT_TAG        5.5
                    GIT_SHALLOW    TRUE
                )
                FetchContent_MakeAvailable(raylib)
                set(RAYLIB_FOUND_METHOD "fetch")
            else()
                message(FATAL_ERROR
                    "raylib was not found. Install it (e.g. `sudo apt install libraylib-dev`, "
                    "`brew install raylib`, or `vcpkg install raylib`) and re-run cmake, "
                    "or re-run with -DCHACRAFT_FETCH_RAYLIB=ON to download it automatically.")
            endif()
        endif()
    endif()
endif()

# ---------------------------------------------------------------------------
# Targets
# ---------------------------------------------------------------------------
add_executable(cha_craft
    src/main.cpp
    src/Customer.cpp
    src/CustomerLine.cpp
    src/CupStack.cpp
    src/Game.cpp
    src/IngredientCarousel.cpp
    src/Input.cpp
    src/RecipeBook.cpp
    src/Renderer.cpp
)

target_include_directories(cha_craft
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link raylib according to which discovery method worked.
if(RAYLIB_FOUND_METHOD STREQUAL "cmake")
    target_link_libraries(cha_craft PRIVATE raylib)
elseif(RAYLIB_FOUND_METHOD STREQUAL "pkg-config")
    target_link_libraries(cha_craft PRIVATE PkgConfig::RAYLIB)
elseif(RAYLIB_FOUND_METHOD STREQUAL "manual")
    target_include_directories(cha_craft PRIVATE ${RAYLIB_INCLUDE_DIR})
    target_link_libraries(cha_craft PRIVATE ${RAYLIB_LIBRARY})
    # raylib on Linux links against these system libraries.
    if(UNIX AND NOT APPLE)
        target_link_libraries(cha_craft PRIVATE GL m pthread dl rt X11)
    endif()
elseif(RAYLIB_FOUND_METHOD STREQUAL "fetch")
    target_link_libraries(cha_craft PRIVATE raylib)
endif()

# raylib uses GNU-ish extensions on non-MSVC compilers; turn off a couple of
# warnings that would otherwise be noisy with -Wall -Wextra.
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(cha_craft PRIVATE
        -Wall -Wextra -Wno-unused-parameter -Wno-unused-but-set-variable)
endif()

set_target_properties(cha_craft PROPERTIES
    OUTPUT_NAME "cha_craft"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

# ---------------------------------------------------------------------------
# Install
# ---------------------------------------------------------------------------
include(GNUInstallDirs)
install(TARGETS cha_craft RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
