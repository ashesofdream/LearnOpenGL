project(Advance-CubeShadowMapping)
find_path(Advance-CubeShadowMapping "stb_image.h")

add_executable(Advance-CubeShadowMapping main.cpp ../util.cpp ../Shader.cpp ../Mesh.cpp ../Mesh.h ../Model.cpp)

file(CREATE_LINK ${CMAKE_SOURCE_DIR}/resource ${CMAKE_BINARY_DIR}/resource SYMBOLIC)
file(CREATE_LINK ${CMAKE_CURRENT_SOURCE_DIR}/shaders ${CMAKE_CURRENT_BINARY_DIR}/shaders SYMBOLIC)
set_property(TARGET Advance-CubeShadowMapping  PROPERTY CXX_STANDARD 17)

target_link_libraries(Advance-CubeShadowMapping glfw glad::glad assimp::assimp)
target_include_directories(Advance-CubeShadowMapping PRIVATE ${STB_INCLUDE_DIRS})
