cat > src/infrastructure/CMakeLists.txt << 'EOF'
# nvinfra - Infrastructure layer (serialization + media)
# Dependencies: nlohmann_json, libzip, FFmpeg

set(NVINFRA_SOURCES
    serialization/zip_project_serializer.cpp
    serialization/zip_project_deserializer.cpp
    media/media_probe.cpp
    media/thumbnail_generator.cpp
    media/media_validator.cpp
    media/media_importer.cpp
)

set(NVINFRA_HEADERS
    serialization/zip_project_serializer.h
    serialization/zip_project_deserializer.h
    media/media_probe.h
    media/thumbnail_generator.h
    media/media_validator.h
    media/media_importer.h
)

add_library(nvinfra STATIC ${NVINFRA_SOURCES} ${NVINFRA_HEADERS})

target_link_libraries(nvinfra
    PUBLIC
        nvcore
        nlohmann_json::nlohmann_json
    PRIVATE
        ${AVFORMAT_LIBRARIES}
        ${AVCODEC_LIBRARIES}
        ${AVUTIL_LIBRARIES}
        ${SWSCALE_LIBRARIES}
        ${LIBZIP_LIBRARIES}
)

# FIX: Ekspos direktori ini secara PUBLIC agar nvtests bisa menemukan header serialization/
target_include_directories(nvinfra
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
        ${AVFORMAT_INCLUDE_DIRS}
        ${AVCODEC_INCLUDE_DIRS}
        ${AVUTIL_INCLUDE_DIRS}
        ${SWSCALE_INCLUDE_DIRS}
        ${LIBZIP_INCLUDE_DIRS}
)
EOF
