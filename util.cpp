#include "util.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include<glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ostream>
#include <stdexcept>
#define STBI_WINDOWS_UTF8
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <filesystem>
#include <memory>
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
    if(glfwGetKey(window,GLFW_KEY_ESCAPE)==GLFW_PRESS){
        glfwSetWindowShouldClose(window, GLFW_TRUE);
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

unsigned int util::texture_from_file(const char *filename, const char *directory) {
    int width,height,channels;
    filesystem::path full_path = directory;
    //full_path =  absolute(full_path);
    full_path.append(filename);
    if(!filesystem::exists(full_path)){
        throw runtime_error("File does not exists");
    }
    int buff_len = full_path.string().size()*3+1;
    auto buff = make_unique<char[]>(buff_len);;
    stbi_convert_wchar_to_utf8(buff.get(),buff_len,full_path.c_str());
    auto texture_data = stbi_load(buff.get(),&width,&height,&channels,0);
    if(texture_data == nullptr) throw  std::runtime_error("Could not load texture from"s+full_path.string());

    unsigned  int texture_id = 0;
    glGenTextures(1,&texture_id);
    glBindTexture(GL_TEXTURE_2D,texture_id);
    int image_format = 0;
    if(channels == 3) image_format = GL_RGB;
    else if(channels == 4) image_format = GL_RGBA;
    else if(channels == 1) image_format = GL_ALPHA;
    glTexImage2D(GL_TEXTURE_2D,0,image_format,width,height,0,image_format,GL_UNSIGNED_BYTE,texture_data);
    glGenerateMipmap(GL_TEXTURE_2D);
    return texture_id;
}

std::array<unsigned int, 2> util::GenVBOVAOAndBind(unsigned int buffer_type){
    unsigned int VAO,VBO;
    glGenBuffers(1,&VBO);
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);
    glBindBuffer(buffer_type,VBO);
    return {VBO,VAO}; 
    
}

unsigned int util::loadCubemap(std::vector<std::string> faces){
    unsigned int textureID;
    glGenTextures(1,&textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP,textureID);
    int width , height ,channels;
    for(int i = 0 ; i < faces.size() ; ++i ){
        auto image_data = stbi_load(faces[i].c_str(), &width, &height, &channels, 0);
        if(image_data==nullptr) throw runtime_error("file: "s+faces[i].c_str()+" is not exist");
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i , 0 , GL_RGB , width , height , 0 , GL_RGB , GL_UNSIGNED_BYTE ,image_data);
        stbi_image_free(image_data);
    }
    return textureID;
}

