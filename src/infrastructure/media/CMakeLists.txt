set(NVMEDIA_SOURCES
    media_probe.cpp
    media_validator.cpp
    media_importer.cpp
    thumbnail_generator.cpp
)

set(NVMEDIA_HEADERS
    media_probe.h
    media_validator.h
    media_importer.h
    thumbnail_generator.h
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
