add_subdirectory(gfx-base)
add_subdirectory(gfx-empty)
add_subdirectory(gfx-agent)
add_subdirectory(gfx-validator)
add_subdirectory(frame-graph)

add_subdirectory(gfx-gles-common)
add_subdirectory(gfx-gles2)
add_subdirectory(gfx-gles3)
add_subdirectory(gfx-vulkan)

if(APPLE)
    add_subdirectory(gfx-metal)
endif()
