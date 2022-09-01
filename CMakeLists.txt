cmake_minimum_required(VERSION 3.0.0)
project(LearnOpen VERSION 0.1.0)
find_package(glfw3 CONFIG REQUIRED)
find_package(Eigen3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
include(CTest)
enable_testing()

add_executable(LearnOpen main.cpp HelloTriangle.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
target_link_libraries(LearnOpen glfw glad::glad)
include(CPack)
