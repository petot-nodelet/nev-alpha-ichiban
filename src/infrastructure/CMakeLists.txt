# nvinfra: Infrastructure library
# Serialization, File I/O, Cache
# Depends on: nvcore, nlohmann_json, libzip

set(NVINFRA_SOURCES
    serialization/zip_project_serializer.cpp
    serialization/zip_project_deserializer.cpp
)

set(NVINFRA_HEADERS
    include/nvinfra/serialization/zip_project_serializer.h
)

add_library(nvinfra STATIC
    ${NVINFRA_SOURCES}
    ${NVINFRA_HEADERS}
)

target_link_libraries(nvinfra
    PUBLIC
        nvcore
        nlohmann_json::nlohmann_json
        ${LIBZIP_LIBRARIES}
)

target_include_directories(nvinfra
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${LIBZIP_INCLUDE_DIRS}
)

target_compile_options(nvinfra
    PRIVATE
        ${LIBZIP_CFLAGS_OTHER}
)
