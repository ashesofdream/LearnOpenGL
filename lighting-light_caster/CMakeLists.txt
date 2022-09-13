project(lighting-light_caster)
find_path(STB_INCLUDE_DIRS "stb_image.h")

add_executable(lighting-light_caster main.cpp ../util.cpp ../Shader.cpp)
target_link_libraries(lighting-light_caster glfw glad::glad)
target_include_directories(lighting-light_caster PRIVATE ${STB_INCLUDE_DIRS})
