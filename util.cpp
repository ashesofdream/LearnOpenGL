#include "util.h"
#include <exception>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <windows.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
using namespace std;
GLFWwindow* util::prepare_window() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "failed to create window" << std::endl;
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "failed to inti GLAD" << std::endl;
        return NULL;
    }
    glViewport(0, 0, 800, 600);
    //glfwSetFramebufferSizeCallback(window, resizeCallbackFunc);

    return window;
}
void util::process_input(GLFWwindow* window,glm::vec3 &camera_pos,const glm::vec3 & camera_front,const glm::vec3 &camera_up){

    static float last_frame_time = glfwGetTime();
    float current_frame_time = glfwGetTime();
    float delta_time = current_frame_time - last_frame_time  ;
    last_frame_time = current_frame_time;
    float camera_speed = 2.5f * delta_time;
    //auto ret = glm::normalize(glm::cross(camera_front, camera_up));
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        camera_pos += camera_speed*camera_front;
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        camera_pos -= camera_speed*camera_front;
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        camera_pos -= glm::normalize(glm::cross(camera_front, camera_up))*camera_speed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        camera_pos += glm::normalize(glm::cross(camera_front, camera_up))*camera_speed;
    }
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
        camera_pos += camera_up*camera_speed;
    }
    if(glfwGetKey(window,GLFW_KEY_LEFT_SHIFT)== GLFW_PRESS){
        camera_pos -= camera_up*camera_speed;
    }
}

glm::vec3 * p_camera_front = nullptr;

float yaw;
float pitch;
void mouse_callback(GLFWwindow* window,double x,double y){
    glm::vec3& camera_front = (*p_camera_front);

    static  bool first_mouse = true;
    static  double last_x = 0;
    static double last_y = 0;
    static float sensitivity = 0.05f;
    //cout<<"x:"<<x<<" y:"<<y<<endl;
    if(first_mouse){
        first_mouse = false;
        last_x = x;
        last_y = y;
        //return;
    }
    double x_offset =  x - last_x;
    double y_offset = last_y - y ;
    last_x = x;
    last_y = y;
    
    yaw += static_cast<float>(x_offset)*sensitivity;
    pitch += static_cast<float>(y_offset)*sensitivity;
    if(pitch>89.f) pitch = 89.f;
    else if(pitch<-89.f) pitch = -89.f;
    camera_front.y = glm::sin(glm::radians(pitch));
    camera_front.x = glm::cos(glm::radians(pitch))*glm::cos(glm::radians(yaw));
    camera_front.z = glm::cos(glm::radians(pitch))*glm::sin(glm::radians(yaw));

    camera_front = glm::normalize(camera_front);
}


void util::init_mouse(GLFWwindow* window,glm::vec3& camera_front,float yaw_, float pitch_){
    if(!window) throw std::runtime_error("window is nullptr");
    yaw = yaw_;
    pitch = pitch_;

    p_camera_front = &camera_front;
    glfwSetCursorPosCallback(window,mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void util::debug(const char *message) {
    cout << message << endl;
}

unsigned int util::load_image_as_texture(const char *path) {
    stbi_load(path)
}
