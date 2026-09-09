set(NVAPP_SOURCES
    main.cpp
    application.cpp
)

add_executable(nirvanaedit ${NVAPP_SOURCES})

target_link_libraries(nirvanaedit
    PRIVATE
        nvcore
        nvinfra
        ${GTK3_LIBRARIES}
)

target_include_directories(nirvanaedit
    PRIVATE
        ${GTK3_INCLUDE_DIRS}
)

install(TARGETS nirvanaedit
    RUNTIME DESTINATION bin
)
