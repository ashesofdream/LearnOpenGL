project(Advance-DefferedShading)
find_path(Advance-DefferedShading "stb_image.h")

add_executable(Advance-DefferedShading main.cpp ../util.cpp ../Shader.cpp ../Mesh.cpp ../Mesh.h ../Model.cpp ../Camera.cpp)

file(CREATE_LINK ${CMAKE_SOURCE_DIR}/resource ${CMAKE_BINARY_DIR}/resource SYMBOLIC)
file(CREATE_LINK ${CMAKE_SOURCE_DIR}/util ${CMAKE_BINARY_DIR}/util SYMBOLIC)
file(CREATE_LINK ${CMAKE_CURRENT_SOURCE_DIR}/shaders ${CMAKE_CURRENT_BINARY_DIR}/shaders SYMBOLIC)
set_property(TARGET Advance-DefferedShading  PROPERTY CXX_STANDARD 17)

target_link_libraries(Advance-DefferedShading glfw glad::glad assimp::assimp)
target_include_directories(Advance-DefferedShading PRIVATE ${STB_INCLUDE_DIRS})
