# Required dependencies for NirvanaEdit
find_package(PkgConfig REQUIRED)

# GTK3 — UI toolkit (required)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0>=3.22)
include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
add_definitions(${GTK3_CFLAGS_OTHER})

# Phase 1: JSON Parser (Header-only, used in infrastructure)
find_package(nlohmann_json REQUIRED)

# Phase 1: ZIP Handler (C library, used in infrastructure)
pkg_check_modules(LIBZIP REQUIRED libzip)
include_directories(${LIBZIP_INCLUDE_DIRS})
link_directories(${LIBZIP_LIBRARY_DIRS})

message(STATUS "GTK3 version: ${GTK3_VERSION}")
message(STATUS "nlohmann_json found: ${nlohmann_json_FOUND}")
message(STATUS "libzip version: ${LIBZIP_VERSION}")