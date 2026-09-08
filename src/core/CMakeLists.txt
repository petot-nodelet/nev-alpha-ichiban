set(NVCORE_SOURCES
    src/version.cpp
    src/project/project.cpp
    src/project/project_info.cpp
    src/project/asset.cpp
    src/serialization/schema_version.cpp
)

set(NVCORE_HEADERS
    include/nvcore/version.h
    include/nvcore/types.h
    include/nvcore/result.h
    include/nvcore/id.h
    include/nvcore/project/project.h
    include/nvcore/project/project_info.h
    include/nvcore/project/project_settings.h
    include/nvcore/project/asset.h
    include/nvcore/project/asset_metadata.h
    include/nvcore/project/clip.h
    include/nvcore/project/track.h
    include/nvcore/project/timeline.h
    include/nvcore/project/composition.h
    include/nvcore/serialization/project_serializer.h
    include/nvcore/serialization/project_deserializer.h
    include/nvcore/serialization/schema_version.h
)

add_library(nvcore STATIC ${NVCORE_SOURCES} ${NVCORE_HEADERS})

target_include_directories(nvcore
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(nvcore PUBLIC cxx_std_17)
