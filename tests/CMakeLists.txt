set(NVTEST_SOURCES
    test_main.cpp
    core/test_version.cpp
    core/test_project.cpp
    core/test_asset.cpp
    core/test_serialization.cpp
)
add_executable(nvtests ${NVTEST_SOURCES})
target_link_libraries(nvtests PRIVATE nvcore nvinfra)
target_include_directories(nvtests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
add_test(NAME NirvanaEditTests COMMAND nvtests)
