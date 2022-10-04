project(Advance-ShadowMapping)
find_path(Advance-ShadowMapping "stb_image.h")

add_executable(Advance-ShadowMapping main.cpp ../util.cpp ../Shader.cpp ../Mesh.cpp ../Mesh.h ../Model.cpp)

file(CREATE_LINK ${CMAKE_SOURCE_DIR}/resource ${CMAKE_BINARY_DIR}/resource SYMBOLIC)
file(CREATE_LINK ${CMAKE_CURRENT_SOURCE_DIR}/shaders ${CMAKE_CURRENT_BINARY_DIR}/shaders SYMBOLIC)
set_property(TARGET Advance-ShadowMapping  PROPERTY CXX_STANDARD 17)

target_link_libraries(Advance-ShadowMapping glfw glad::glad assimp::assimp)
target_include_directories(Advance-ShadowMapping PRIVATE ${STB_INCLUDE_DIRS})
