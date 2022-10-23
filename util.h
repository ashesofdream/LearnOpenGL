#pragma once
#include <array>
#include <string>
#include <vector>
struct GLFWwindow;
#include <glm/glm.hpp>
#include <glad/glad.h>

namespace util {
	GLFWwindow* prepare_window();
	void process_input(GLFWwindow* window,glm::vec3 &camera_pos,const glm::vec3 & camera_front,const glm::vec3 &camera_up);
	void init_mouse(GLFWwindow* window,glm::vec3& camera_front,float yaw_=270.f, float pitch_=0.f);
    void debug(const char* message);
    unsigned int texture_from_file(const char* filename,const char* directory);
	std::array<unsigned int, 2> GenVBOVAOAndBind(unsigned int buffer_type=0x8892);//defalut is GL_ARRAY_BUFFER
	unsigned int loadCubemap(std::vector<std::string> faces);
	void draw_quad();
	void set_texture_prop(GLint scale=NULL,GLint wrap=NULL,bool is_3d = false);

}