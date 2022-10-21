#include "Camera.h"
#include "util.h"
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <ostream>


void default_process_func(Camera* camera){
    util::process_input(camera->window(),camera->pos(),camera->front(),camera->up());
}

Camera::Camera():front_(glm::vec3(0.f,0.f,-1.f)),up_(0.f,1.f,0.f),pos_(0.f,0.f,1.f),fov_(45.f),aspect_(static_cast<float>(800)/600),near_(0.1f),far_(100.f),window_(nullptr),process_func_(default_process_func)
{
}
Camera::Camera(GLFWwindow* window):Camera()
{
    window_ =window;
    util::init_mouse(window, front_);  
}

glm::vec3& Camera::front(){
    return front_;
}

glm::vec3& Camera::up(){
    return up_;
}

glm::vec3& Camera::pos(){
    return pos_;
}
GLFWwindow* Camera::window(){
    return window_;
}

void Camera::update(){
    process_func_(this);
}

glm::mat4 Camera::perspective_matrix(){
    return glm::perspective(glm::radians(fov_),aspect_, near_, far_);
}

glm::mat4 Camera::view_matrix(){
    return glm::lookAt(pos_, pos_+front_, up_);
}