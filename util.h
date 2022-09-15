#pragma once
struct GLFWwindow;
#include <glm/glm.hpp>

namespace util {
	GLFWwindow* prepare_window();
	void process_input(GLFWwindow* window,glm::vec3 &camera_pos,const glm::vec3 & camera_front,const glm::vec3 &camera_up);
	void init_mouse(GLFWwindow* window,glm::vec3& camera_front,float yaw_=270.f, float pitch_=0.f);
    void debug(const char* message);
    unsigned int texture_from_file(const char* filename,const char* directory);
}