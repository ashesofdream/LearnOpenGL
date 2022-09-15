project(lighting-multiple_lights)
find_path(STB_INCLUDE_DIRS "stb_image.h")

add_executable(lighting-multiple_lights main.cpp ../util.cpp ../Shader.cpp ../Mesh.cpp ../Mesh.h)
target_link_libraries(lighting-multiple_lights glfw glad::glad)
target_include_directories(lighting-multiple_lights PRIVATE ${STB_INCLUDE_DIRS})
