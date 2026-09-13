cat > tests/CMakeLists.txt << 'EOF'
set(NVTEST_SOURCES
    test_main.cpp
    core/test_version.cpp
    core/test_project.cpp
    core/test_asset.cpp
    core/test_serialization.cpp
    infrastructure/media/test_media_validator.cpp
    infrastructure/media/test_media_probe.cpp
)

add_executable(nvtests ${NVTEST_SOURCES})

target_link_libraries(nvtests
    PRIVATE
        nvcore
        nvinfra
)

target_include_directories(nvtests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/src/infrastructure
)

add_test(NAME NirvanaEditTests COMMAND nvtests)
EOF
