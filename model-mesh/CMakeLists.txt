project(model-mesh)
find_path(STB_INCLUDE_DIRS "stb_image.h")
message(WARNING  ${CMAKE_CURRENT_SOURCE_DIR}/shaders/)
add_executable(model-mesh main.cpp ../util.cpp ../Shader.cpp ../Model.cpp  ../Mesh.cpp)
file(GLOB RESOURCE_FILE ${CMAKE_SOURCE_DIR}/resource/*)
add_custom_command(
        TARGET model-mesh PRE_BUILD
        COMMAND
        ${CMAKE_COMMAND} -E make_directory
        ${CMAKE_BINARY_DIR}/resource/)
add_custom_command(
        TARGET model-mesh POST_BUILD
        COMMAND
        ${CMAKE_COMMAND} -E  copy_if_different
        ${RESOURCE_FILE}
        ${CMAKE_BINARY_DIR}/resource/
)
add_custom_command(
        TARGET model-mesh POST_BUILD
        COMMAND
        ${CMAKE_COMMAND} -E make_directory
        ${CMAKE_CURRENT_BINARY_DIR}/shaders/)
file(GLOB SHADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.glsl)
add_custom_command(
        TARGET model-mesh POST_BUILD
        COMMAND
        ${CMAKE_COMMAND} -E  copy_if_different
        ${SHADER_FILES}
        ${CMAKE_CURRENT_BINARY_DIR}/shaders/

)


set_property(TARGET model-mesh  PROPERTY CXX_STANDARD 17)
target_link_libraries(model-mesh glfw glad::glad assimp::assimp)
target_include_directories(model-mesh PRIVATE ${STB_INCLUDE_DIRS})
