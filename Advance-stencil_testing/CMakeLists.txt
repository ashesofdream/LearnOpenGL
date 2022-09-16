project(stencil_test)
find_path(STB_INCLUDE_DIRS "stb_image.h")

add_executable(stencil_test main.cpp ../util.cpp ../Shader.cpp ../Mesh.cpp ../Mesh.h)
file(GLOB RESOURCE_FILES ${CMAKE_SOURCE_DIR}/resource/*)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/resource/)
foreach( resource_file ${RESOURCE_FILES})
    get_filename_component(resource_filename ${resource_file} NAME )
    file(COPY_FILE ${resource_file} ${CMAKE_BINARY_DIR}/resource/${resource_filename} ONLY_IF_DIFFERENT)
endforeach()
file(GLOB SHADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.glsl)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/shaders/)
foreach( shader_file ${SHADER_FILES})
    get_filename_component(shader_filename ${shader_file} NAME)
    file(COPY_FILE ${shader_file}  ${CMAKE_CURRENT_BINARY_DIR}/shaders/${shader_filename} ONLY_IF_DIFFERENT)
endforeach()
add_custom_target(
        EchoTest ALL
        COMMAND
        "pwd"
)

set_property(TARGET stencil_test  PROPERTY CXX_STANDARD 17)

target_link_libraries(stencil_test glfw glad::glad)
target_include_directories(stencil_test PRIVATE ${STB_INCLUDE_DIRS})
