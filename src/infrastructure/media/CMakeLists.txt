set(NVMEDIA_SOURCES
    media_validator.cpp
    media_probe.cpp
)

set(NVMEDIA_HEADERS
    media_validator.h
    media_probe.h
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
