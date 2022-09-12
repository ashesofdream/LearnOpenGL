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

void process_input(GLFWwindow* window,glm::vec3 &camera_pos,const glm::vec3 & camera_front,const glm::vec3 &camera_up){
    float camera_speed = 0.05f;
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
}

float yaw = 270.f;
float pitch = 0.f;
glm::vec3 camera_front {glm::cos(glm::radians(pitch))*glm::cos(glm::radians(yaw)),
glm::sin(glm::radians(pitch)),
glm::cos(glm::radians(pitch))*glm::sin(glm::radians(yaw))};
void mouse_callback(GLFWwindow* window,double x,double y){
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

int main(){
    auto window = util::prepare_window();

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
    Shader shader("D://Code/graphic/LearnOpenGL/Camera/shader.vs","D://Code/graphic/LearnOpenGL/Camera/shader.fs");
  
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

    // glGenBuffers(1,&EBO);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(index),index,GL_STATIC_DRAW);

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
    model_matrix = glm::rotate(model_matrix, glm::radians(0.f),glm::vec3(1.f,0.f,0.f));
    // glm::mat4 view_matrix(1.f);
    // view_matrix = glm::translate(view_matrix,glm::vec3(0.f,0.f,-3.f));
    glm::mat4 projection_matrix = glm::perspective(45.f, float(width)/float(height), 0.1f, 100.f);
    
    glEnable(GL_DEPTH_TEST);
    glm::vec3 cam_pos {0.f,0.f,3.f};

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    while(!glfwWindowShouldClose(window)){   
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_obj);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,texture_obj_2);
        shader.use();
        glBindVertexArray(VAO); 
        // float radius = 10;
        // float cam_x = radius*glm::cos(glfwGetTime());
        // float cam_z = radius*glm::sin(glfwGetTime());
        //glm::mat4 view_matrix = glm::lookAt(glm::vec3(cam_x,0.f,cam_z),glm::vec3(0.f),glm::vec3(0.f,1.f,0.f));
        process_input(window, cam_pos, camera_front,{0.f,1.f,0.f});
        glm::mat4 view_matrix = glm::lookAt(cam_pos, cam_pos+camera_front*glm::vec3(3), {0.f,1.f,0.f});
        glUniformMatrix4fv(glGetUniformLocation(shader.program_id,"mvp"),1,GL_FALSE,glm::value_ptr(projection_matrix*view_matrix*model_matrix));
        //glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
        for(int i = 0 ; i < 10 ; ++i ){
            glm::mat4 trans(1.f);
            trans = glm::translate(trans, cubePositions[i]);
            if(i==0||i%3==2){
                if(degree>360.f) degree -= 360.f;
                trans = glm::rotate(trans, glm::radians(degree),{0.f,0.f,1.f});
            }
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
