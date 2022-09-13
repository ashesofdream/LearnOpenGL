project(color_texture)
find_path(STB_INCLUDE_DIRS "stb_image.h")

add_executable(lighting-color_texture main.cpp ../util.cpp ../Shader.cpp)
target_link_libraries(lighting-color_texture glfw glad::glad)
target_include_directories(lighting-color_texture PRIVATE ${STB_INCLUDE_DIRS})
