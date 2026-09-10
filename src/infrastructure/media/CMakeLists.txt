# nvmedia - Media Subsystem (Phase 2)
# Dependencies: FFmpeg (libavformat, libavcodec, libavutil, libswscale)

set(NVMEDIA_SOURCES
    media_probe.cpp
    thumbnail_generator.cpp
    media_validator.cpp
    media_importer.cpp
)

set(NVMEDIA_HEADERS
    media_probe.h
    thumbnail_generator.h
    media_validator.h
    media_importer.h
)

add_library(nvmedia STATIC ${NVMEDIA_SOURCES} ${NVMEDIA_HEADERS})

target_link_libraries(nvmedia
    PUBLIC
        nvcore
    PRIVATE
        ${AVFORMAT_LIBRARIES}
        ${AVCODEC_LIBRARIES}
        ${AVUTIL_LIBRARIES}
        ${SWSCALE_LIBRARIES}
)

target_include_directories(nvmedia
    PRIVATE
        ${AVFORMAT_INCLUDE_DIRS}
        ${AVCODEC_INCLUDE_DIRS}
        ${AVUTIL_INCLUDE_DIRS}
        ${SWSCALE_INCLUDE_DIRS}
)
