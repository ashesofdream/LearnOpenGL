project(lighting-color)

add_executable(lighting-color main.cpp ../util.cpp ../Shader.cpp)
target_link_libraries(lighting-color glfw glad::glad)