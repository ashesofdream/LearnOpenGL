#include "Camera.h"
#include "util.h"

void default_process_func(Camera* camera){
    util::process_input(camera->window(),camera->pos(),camera->front(),camera->up());
}

Camera::Camera():front_(glm::vec3(1.f,0,0)),up_(0.f,1.f,0.f),pos_(0.f),process_func_(default_process_func)
{
}
Camera::Camera(GLFWwindow* window):front_(glm::vec3(1.f,0,0)),up_(0.f,1.f,0.f),pos_(0.f),process_func_(default_process_func)
{
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

void Camera::update(){
    process_func_(this);
}
