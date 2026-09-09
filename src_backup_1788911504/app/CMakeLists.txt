# nvapp: Application entry point (GTK3)
# Depends on: nvcore, GTK3

set(NVAPP_SOURCES
    main.cpp
    application.cpp
)

add_executable(nirvanaedit ${NVAPP_SOURCES})

target_link_libraries(nirvanaedit
    PRIVATE
        nvcore
        ${GTK3_LIBRARIES}
)

target_include_directories(nirvanaedit
    PRIVATE
        ${GTK3_INCLUDE_DIRS}
)

# Install target (for packaging later)
install(TARGETS nirvanaedit
    RUNTIME DESTINATION bin
)
