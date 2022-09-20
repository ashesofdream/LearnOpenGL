project(FaceCulling)
find_path(STB_INCLUDE_DIRS "stb_image.h")

add_executable(FaceCulling main.cpp ../util.cpp ../Shader.cpp ../Mesh.cpp ../Mesh.h)

file(CREATE_LINK ${CMAKE_SOURCE_DIR}/resource ${CMAKE_BINARY_DIR}/resource SYMBOLIC)
file(CREATE_LINK ${CMAKE_CURRENT_SOURCE_DIR}/shaders ${CMAKE_CURRENT_BINARY_DIR}/shaders SYMBOLIC)
set_property(TARGET FaceCulling  PROPERTY CXX_STANDARD 17)

target_link_libraries(FaceCulling glfw glad::glad)
target_include_directories(FaceCulling PRIVATE ${STB_INCLUDE_DIRS})
