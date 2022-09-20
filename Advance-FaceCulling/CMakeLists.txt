project(blending)
find_path(STB_INCLUDE_DIRS "stb_image.h")

add_executable(blending main.cpp ../util.cpp ../Shader.cpp ../Mesh.cpp ../Mesh.h)
#file(GLOB RESOURCE_FILES ${CMAKE_SOURCE_DIR}/resource/*)
#file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/resource/)
#foreach( resource_file ${RESOURCE_FILES})
#    MESSAGE(WARNING ${resource_file} )
#    get_filename_component(resource_filename ${resource_file} NAME )
#    file(COPY_FILE ${resource_file} ${CMAKE_BINARY_DIR}/resource/${resource_filename} ONLY_IF_DIFFERENT)
#endforeach()
#file(GLOB SHADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.glsl)
#file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/shaders/)
#foreach( shader_file ${SHADER_FILES})
#    get_filename_component(shader_filename ${shader_file} NAME)
#    file(COPY_FILE ${shader_file}  ${CMAKE_CURRENT_BINARY_DIR}/shaders/${shader_filename} ONLY_IF_DIFFERENT)
#endforeach()
#add_custom_target(
#        EchoTest ALL
#        COMMAND
#        "pwd"
#)
file(CREATE_LINK ${CMAKE_SOURCE_DIR}/resource ${CMAKE_BINARY_DIR}/resource SYMBOLIC)
file(CREATE_LINK ${CMAKE_CURRENT_SOURCE_DIR}/shaders ${CMAKE_CURRENT_BINARY_DIR}/shaders SYMBOLIC)
set_property(TARGET blending  PROPERTY CXX_STANDARD 17)

target_link_libraries(blending glfw glad::glad)
target_include_directories(blending PRIVATE ${STB_INCLUDE_DIRS})
