project(Advance-AdvanceGLSL)
find_path(Advance-AdvanceGLSL "stb_image.h")

add_executable(Advance-AdvanceGLSL main.cpp ../util.cpp ../Shader.cpp ../Mesh.cpp ../Mesh.h ../Model.cpp)

file(CREATE_LINK ${CMAKE_SOURCE_DIR}/resource ${CMAKE_BINARY_DIR}/resource SYMBOLIC)
file(CREATE_LINK ${CMAKE_CURRENT_SOURCE_DIR}/shaders ${CMAKE_CURRENT_BINARY_DIR}/shaders SYMBOLIC)
set_property(TARGET Advance-AdvanceGLSL  PROPERTY CXX_STANDARD 17)

target_link_libraries(Advance-AdvanceGLSL glfw glad::glad assimp::assimp)
target_include_directories(Advance-AdvanceGLSL PRIVATE ${STB_INCLUDE_DIRS})
