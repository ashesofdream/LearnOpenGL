project(Advance-GeometryShader)
find_path(Advance-GeometryShader "stb_image.h")

add_executable(Advance-GeometryShader main.cpp ../util.cpp ../Shader.cpp ../Mesh.cpp ../Mesh.h ../Model.cpp ../Advance-GeometryShader/main.cpp)

file(CREATE_LINK ${CMAKE_SOURCE_DIR}/resource ${CMAKE_BINARY_DIR}/resource SYMBOLIC)
file(CREATE_LINK ${CMAKE_CURRENT_SOURCE_DIR}/shaders ${CMAKE_CURRENT_BINARY_DIR}/shaders SYMBOLIC)
set_property(TARGET Advance-GeometryShader  PROPERTY CXX_STANDARD 17)

target_link_libraries(Advance-GeometryShader glfw glad::glad assimp::assimp)
target_include_directories(Advance-GeometryShader PRIVATE ${STB_INCLUDE_DIRS})
