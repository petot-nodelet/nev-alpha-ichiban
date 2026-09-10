# NirvanaEdit Dependencies Configuration

find_package(PkgConfig REQUIRED)

# GTK3 — UI toolkit (required)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0>=3.22)
include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
add_definitions(${GTK3_CFLAGS_OTHER})

# nlohmann_json — JSON parser (required for serialization)
find_package(nlohmann_json REQUIRED)

# libzip — ZIP archive handling (required for .nvp format)
pkg_check_modules(LIBZIP REQUIRED libzip)
include_directories(${LIBZIP_INCLUDE_DIRS})
link_directories(${LIBZIP_LIBRARY_DIRS})

# FFmpeg — Media processing (Phase 2+)
pkg_check_modules(AVFORMAT REQUIRED libavformat)
pkg_check_modules(AVCODEC REQUIRED libavcodec)
pkg_check_modules(AVUTIL REQUIRED libavutil)
pkg_check_modules(SWSCALE REQUIRED libswscale)

message(STATUS "GTK3 version: ${GTK3_VERSION}")
message(STATUS "nlohmann_json found: ${nlohmann_json_FOUND}")
message(STATUS "libzip version: ${LIBZIP_VERSION}")
message(STATUS "FFmpeg libavformat version: ${AVFORMAT_VERSION}")
message(STATUS "FFmpeg libavcodec version: ${AVCODEC_VERSION}")
message(STATUS "FFmpeg libavutil version: ${AVUTIL_VERSION}")
message(STATUS "FFmpeg libswscale version: ${SWSCALE_VERSION}")
