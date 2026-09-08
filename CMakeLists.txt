cmake_minimum_required(VERSION 3.10)
project(NirvanaEdit
    VERSION 0.0.1
    DESCRIPTION "Lightweight native Linux video editor"
    LANGUAGES CXX
)

# Global settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Default to Release if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Custom modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(CompilerFlags)
include(Dependencies)

# Options
option(NV_BUILD_TESTS "Build unit tests" ON)
option(NV_ENABLE_SANITIZERS "Enable ASan/UBSan (debug only)" OFF)

# Subdirectories
add_subdirectory(src)

if(NV_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Summary
message(STATUS "=== NirvanaEdit Configuration ===")
message(STATUS "Version       : ${PROJECT_VERSION}")
message(STATUS "Build type    : ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ compiler  : ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "GTK3 found    : ${GTK3_FOUND}")
message(STATUS "Tests         : ${NV_BUILD_TESTS}")
message(STATUS "=================================")
