#include "../Shader.h"
#include "../variable.h"
#include "../util.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>
#include <iostream>


int main(){
    auto window = util::prepare_window();
    int scr_width = 800,scr_height = 600;
    glm::vec3 eye_pos {0.f,0.f,5.f};
    glm::vec3 camera_up {0.f,1.f,0.f};
    glm::vec3 camera_front {0.f,0.f,-1.f};
    auto projection_matrix = glm::perspective(glm::radians(45.f), static_cast<float>(scr_width)/scr_height ,0.1f, 100.f);

    Shader worldShader("shaders/world.vs.glsl","shaders/world.fs.glsl");
    worldShader.use();
    auto &&[cubeVBO,cubeVAO]=util::GenVBOVAOAndBind();
    glBufferData(GL_ARRAY_BUFFER,sizeof(cubeVertices_with_texture),cubeVertices_with_texture,GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(float)*5,reinterpret_cast<void*>(0));
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,sizeof(float)*5,reinterpret_cast<void*>(sizeof(float)*3));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    auto cube_texture = util::texture_from_file("container.jpg", "../resource/");
    worldShader.set_mat4("projection", projection_matrix);
    worldShader.set_int("samp", 0);
    glm::vec3 cube_vertice{0.f,0.f,0.f};
    //glEnable(GL_DEPTH_TEST);
    
    Shader skybox_shader("shaders/skybox.vs.glsl","shaders/skybox.fs.glsl");
    skybox_shader.use();
    auto &&[skyVBO , skyVAO] = util::GenVBOVAOAndBind();
    glBufferData(GL_ARRAY_BUFFER,sizeof(skyboxVertices),skyboxVertices,GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(float)*3,nullptr);
    glEnableVertexAttribArray(0);
    auto sky_texture = util::loadCubemap({
        "../resource/skybox/right.jpg",
        "../resource/skybox/left.jpg",
        "../resource/skybox/top.jpg",
        "../resource/skybox/bottom.jpg",
        "../resource/skybox/front.jpg",
        "../resource/skybox/back.jpg"
    });
    glBindTexture(GL_TEXTURE_CUBE_MAP,sky_texture);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    skybox_shader.set_int("skybox", 0);
    skybox_shader.set_mat4("projection", projection_matrix);
    
    util::init_mouse(window, camera_front);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    // glEnable(GL_STENCIL_TEST);
    while(!glfwWindowShouldClose(window)){
       util::process_input(window, eye_pos, camera_front, camera_up);
        glClearColor(0.0f,0.0f,0.0f,0.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
        
        
        auto view_matrix =  glm::lookAt(eye_pos, eye_pos+camera_front, camera_up);
        skybox_shader.set_mat4("model", 
        //glm::translate(glm::mat4(1), eye_pos)
        glm::mat4(1)
        );

        // glStencilFunc(GL_ALWAYS,1,0xff);        
        // glStencilOp(GL_KEEP,GL_KEEP,GL_INCR);
        worldShader.use();
        worldShader.set_mat4("view", view_matrix);
        worldShader.set_mat4("model",glm::translate(glm::mat4(1.f), cube_vertice));
        glBindVertexArray(cubeVAO);
        glBindTexture(GL_TEXTURE_2D,cube_texture);
        glDrawArrays(GL_TRIANGLES,0,36);


        // glStencilFunc(GL_EQUAL,0,0xff);
        // glStencilMask(0x00);
        skybox_shader.use();    
        // glDepthMask(GL_FALSE);
        skybox_shader.set_mat4("view",glm::mat4(glm::mat3(view_matrix)));
        glBindVertexArray(skyVAO);
        glBindTexture(GL_TEXTURE_CUBE_MAP,sky_texture);
        glActiveTexture(GL_TEXTURE0);
        glDrawArrays(GL_TRIANGLES,0,36);
        // glDepthMask(GL_TRUE);
        // glStencilMask(0xFF);

        
        
        glfwSwapBuffers(window);
        glfwPollEvents();

    }
}