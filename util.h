#pragma once
struct GLFWwindow;
#include <glm/glm.hpp>

namespace util {
	GLFWwindow* prepare_window();
	void process_input(GLFWwindow* window,glm::vec3 &camera_pos,const glm::vec3 & camera_front,const glm::vec3 &camera_up);
	void init_mouse(GLFWwindow* window,glm::vec3& camera_front,float yaw_=270.f, float pitch_=0.f);
    unsigned int load_image_as_texture(const char* path);
    void debug(const char* message);
}