#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../util.h"
#include "../Shader.h"
#include <iostream>
using namespace std;

int main(int argc, char **argv){
    auto window = util::prepare_window();
    Shader shader("D://code//graphic/LearnOpenGL/Shaders/shader.vs.glsl",
    "D://Code/graphic/LearnOpenGL/Shaders//shader.fs.glsl");
    float vertices[] = {
    // 位置              // 颜色
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
    };
    unsigned int VBO,VAO;
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(float)*6,0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(float)*6,(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    while (!glfwWindowShouldClose(window)) {
        shader.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES,0,3);
        glfwSwapBuffers(window);
        glfwPollEvents();        
    }
    
    glfwTerminate();
    return 0;
}