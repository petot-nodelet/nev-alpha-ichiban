set(NVINFRA_SOURCES
    serialization/zip_project_serializer.cpp
    serialization/zip_project_deserializer.cpp
)

set(NVINFRA_HEADERS
    serialization/zip_project_serializer.h
    serialization/zip_project_deserializer.h
)

add_library(nvinfra STATIC ${NVINFRA_SOURCES} ${NVINFRA_HEADERS})

target_link_libraries(nvinfra
    PUBLIC
        nvcore
        nlohmann_json::nlohmann_json
        ${LIBZIP_LIBRARIES}
)

target_include_directories(nvinfra
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${LIBZIP_INCLUDE_DIRS}
)
