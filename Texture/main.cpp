#include "glad/glad.h"
#include "GLFW/glfw3.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "../Shader.h"
#include "../util.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace std;

int main(){
    auto window = util::prepare_window();
    // float vertices[] = {
    // //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
    //     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
    //     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
    //     -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
    //     -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
    // };
    unsigned int index[] = {
        0,1,3,
        1,2,3
    };
    float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f), 
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f)  
    };
    Shader shader("D://Code/graphic/LearnOpenGL/Texture/shader.vs","D://Code/graphic/LearnOpenGL/Texture/shader.fs");
  
    unsigned int texture_obj,texture_obj_2;
    glGenTextures(1,&texture_obj);
    glBindTexture(GL_TEXTURE_2D,texture_obj);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    
    int width,height,nChannels;
    unsigned char *data = stbi_load("D:/Code/graphic/LearnOpenGL/Texture/container.jpg", &width, &height, &nChannels,0);
    if (data == nullptr) {
        std::cout<<"load image fail"<<endl;
        return 0;
    }else{
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    stbi_set_flip_vertically_on_load(true);
    unsigned char *data_2 = stbi_load("D:/Code/graphic/LearnOpenGL/Texture/awesomeface.png", &width, &height, &nChannels,0);
    
    glGenTextures(1,&texture_obj_2);
    glBindTexture(GL_TEXTURE_2D,texture_obj_2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if (data_2 == nullptr) {
        std::cout<<"load image fail"<<endl;
        return 0;
    }else{
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,data_2);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    shader.use();
    glUniform1i(glGetUniformLocation(shader.program_id, "tex"), 0); 
    glUniform1i(glGetUniformLocation(shader.program_id, "tex2"),1);
    
    unsigned int VBO,VAO,EBO;
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

    glGenBuffers(1,&EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(index),index,GL_STATIC_DRAW);

    // glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),0);
    // glEnableVertexAttribArray(0);
    // glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(sizeof(float)*3));
    // glEnableVertexAttribArray(1);
    // glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(sizeof(float)*6));
    // glEnableVertexAttribArray(2);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(float),0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)(sizeof(float)*3));
    glEnableVertexAttribArray(2);
    //matrix
    float degree = 0;
    //begin coords MVP;
    glm::mat4 model_matrix(1.f);
    model_matrix = glm::rotate(model_matrix, glm::radians(-55.f),glm::vec3(1.f,0.f,0.f));
    glm::mat4 view_matrix(1.f);
    view_matrix = glm::translate(view_matrix,glm::vec3(0.f,0.f,-3.f));
    glm::mat4 projection_matrix = glm::perspective(45.f, float(width)/float(height), 0.1f, 100.f);
    glUniformMatrix4fv(glGetUniformLocation(shader.program_id,"mvp"),1,GL_FALSE,glm::value_ptr(projection_matrix*view_matrix*model_matrix));
    glEnable(GL_DEPTH_TEST);
    while(!glfwWindowShouldClose(window)){   
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_obj);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,texture_obj_2);
        shader.use();
        glBindVertexArray(VAO); 
        //glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
        for(int i = 0 ; i < 10 ; ++i ){
            glm::mat4 trans(1.f);
            if(degree>360.f) degree -= 360.f;
            trans = glm::translate(trans, cubePositions[i]);
            trans = glm::rotate(trans, glm::radians(degree),{0.f,0.f,1.f});
            
            glUniformMatrix4fv(glGetUniformLocation(shader.program_id,"trans"),1,GL_FALSE,glm::value_ptr(trans));        
            degree += 0.1f;
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
        glClearColor(0.0f,0.0f,0.0f,0.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    }
    
}
