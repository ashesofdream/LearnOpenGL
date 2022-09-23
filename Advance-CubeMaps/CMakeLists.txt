project(Advance-CubeMaps)
find_path(Advance-CubeMaps "stb_image.h")

add_executable(Advance-CubeMaps main.cpp ../util.cpp ../Shader.cpp ../Mesh.cpp ../Mesh.h)

file(CREATE_LINK ${CMAKE_SOURCE_DIR}/resource ${CMAKE_BINARY_DIR}/resource SYMBOLIC)
file(CREATE_LINK ${CMAKE_CURRENT_SOURCE_DIR}/shaders ${CMAKE_CURRENT_BINARY_DIR}/shaders SYMBOLIC)
set_property(TARGET Advance-CubeMaps  PROPERTY CXX_STANDARD 17)

target_link_libraries(Advance-CubeMaps glfw glad::glad)
target_include_directories(Advance-CubeMaps PRIVATE ${STB_INCLUDE_DIRS})
