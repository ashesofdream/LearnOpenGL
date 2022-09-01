#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ostream>
#include <sal.h>
#include "HelloTriangle.h"

void __callback resizeCallbackFunc(GLFWwindow* window, int width, int height){
    glViewport(0,0,width,height);
    std::cout<<"new size:" << width<<" " <<height<<std::endl;
}

GLFWwindow* prepare_window(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(800,600, "LearnOpenGL", NULL, NULL);
    if(window == NULL){
        std::cout<<"failed to create window"<<std::endl;
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout <<"failed to inti GLAD" <<std::endl;
        return NULL;
    }
    glViewport(0,0,800,600);
    glfwSetFramebufferSizeCallback(window, resizeCallbackFunc);    
    
    return window;
}
void window_loop(GLFWwindow* window){
    while (!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
}
int main(int, char**) {
    auto window = prepare_window();
    if (!window) return 0;
    HelloTriangle::DrawTwoTriangles(window);

}
