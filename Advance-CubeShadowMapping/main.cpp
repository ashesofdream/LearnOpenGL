#include "../Shader.h"
#include "../variable.h"
#include "../util.h"
#include "../Model.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>
#include <iostream>
#include <vector>


using namespace std;




int main(){
    float scr_width = 800,scr_height = 600;
    float shadow_width,shadow_height;shadow_height=shadow_width=1024;

    auto window = util::prepare_window();
    Shader shaders("shaders/vertex.vs.glsl",
    "shaders/fragment.fs.glsl");

    glm::vec3 eye_pos (0.f,0.f,5.f);
    glm::vec3 camera_up {0.f,1.f,0.f};
    glm::vec3 camera_front {0.f,0.f,1.f};
    unsigned int GroundVAO , GroundVBO;
    glGenBuffers(1, &GroundVBO);
    glGenVertexArrays(1, &GroundVAO);

    glBindVertexArray(GroundVAO);
    glBindBuffer(GL_ARRAY_BUFFER,GroundVBO);

    glBufferData(GL_ARRAY_BUFFER,sizeof(planeVertices),&planeVertices,GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(float), nullptr);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)(sizeof(float)*3));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(2);
    
    unsigned int ground_text_id = util::texture_from_file("metal.png","../resource/");
    //model
    Model robot_model("../resource/nanosuit.obj");
    glm::mat4 robot_model_matrix = glm::scale(glm::translate(glm::mat4(1.f),{0.f,-0.4f,0.f}),glm::vec3(0.2) ) ;

    //mvp
    glm::mat4 projection_matrix = glm::perspective(glm::radians(45.f),scr_width/scr_height,1.f,100.f);
    //glm::mat4 projection_matrix = glm::ortho(-1.f,1.f,-1.f,1.f,0.1f,100.f);
    shaders.set_mat4("projection",projection_matrix);
    glm::mat4 e_matrix = glm::mat4 (1.f);

    //from here is shadow mapping content
    unsigned int cube_depth_texture;
    glGenTextures(1, &cube_depth_texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cube_depth_texture);
    for(int i = 0 ; i < 6 ; ++i ){
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,0,GL_DEPTH_COMPONENT,shadow_width,shadow_height,0,GL_DEPTH_COMPONENT,GL_FLOAT,nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

    unsigned int depth_fbo;
    glGenFramebuffers(1, &depth_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, depth_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,cube_depth_texture,0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    
    glBindFramebuffer(GL_FRAMEBUFFER,0);

    //light space
    glm::vec3 light_pos(-2.0f, 4.0f, -1.0f);
    float far_plane = 100.f;
    auto light_space_projection_matrix = glm::perspective(glm::radians(90.f), scr_width/scr_height, 0.1f, far_plane);
    vector<glm::mat4> LightMatrices;
    {
        vector<glm::vec3> cam_dir{{1.f,0.f,0.f},{-1.f,0.f,0.f},{0.f,1.f,0.f},{0.f,-1.f,0.f},{0.f,0.f,1.f},{0.f,0.f,-1.f}};
        vector<glm::vec3> cam_top{{0.f,1.f,0.f},{0.f,1.f,0.f},{0.f,0.f,1.f},{0.f,0.f,-1.f},{0.f,1.f,0.f},{0.f,1.f,0.f}};
        for(int i = 0 ; i <6 ;++i) LightMatrices.push_back(glm::lookAt(eye_pos, eye_pos+cam_dir[i], cam_top[i]));
    }
    auto light_space_view_matrix = glm::lookAt(glm::vec3(-2.0f, 2.0f, -1.0f),glm::vec3 (0.f,0.f,0.f),glm::vec3 (0.f,1.f,0.f));
    Shader depth_shader("shaders/depth_shader.vs.glsl","shaders/depth_shader.fs.glsl");
    depth_shader.use();
    depth_shader.set_mat4("projection",light_space_projection_matrix);
    depth_shader.set_mat4("view",light_space_view_matrix);
    depth_shader.set_arrays_mat4(6,"shadowMatrices",nullptr,LightMatrices);
    

    //show shader texture;
    Shader direct_shader("shaders/direct_buffer_out.vs.glsl","shaders/direct_buffer_out.fs.glsl");
    float show_texture_array[] = {-1.f,1.f,0.f,1.f,
                                  1.f, 1.f,1.f,1.f,
                                   -1.f,-1.f,0.f,0.f,

                                   -1.f,-1.f,0.f,0.f,
                                   1.f,-1.f,1.f,0.f,
                                   1.f,1.f,1.f,1.f};

    auto&&[show_texture_vbo,show_texture_vao] = util::GenVBOVAOAndBind();
    glBufferData(GL_ARRAY_BUFFER, sizeof(show_texture_array),show_texture_array,GL_STATIC_DRAW);
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,sizeof(float)*4,nullptr);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,sizeof(float)*4,reinterpret_cast<void*>(sizeof (float)*2));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    direct_shader.set_int("samp",0);
    //begin prepare for  render shadow
    shaders.use();
    shaders.set_mat4("LightSpaceTrans",light_space_projection_matrix*light_space_view_matrix);
    shaders.set_int("depth_map",1);

    auto draw_func = [&](Shader& shader_){
        shader_.set_mat4("model", glm::mat4(1.0f));
        glBindVertexArray(GroundVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,ground_text_id);
        glDrawArrays(GL_TRIANGLES,0,6);
        shader_.set_mat4("model",robot_model_matrix);
        robot_model.Draw(shader_);
    };
    util::init_mouse(window,camera_front);
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window)) {
        util::process_input(window,eye_pos,camera_front ,camera_up);

        glBindFramebuffer(GL_FRAMEBUFFER,depth_fbo);
        glClear(GL_DEPTH_BUFFER_BIT);
        depth_shader.use();
        draw_func(depth_shader);
        glBindFramebuffer(GL_FRAMEBUFFER,0);

//        direct_shader.use();
//        glDisable(GL_DEPTH_TEST);
//        glBindVertexArray(show_texture_vao);
//        glBindTexture(GL_TEXTURE_2D,depth_texture);
//        glActiveTexture(GL_TEXTURE0);
//        glDrawArrays(GL_TRIANGLES,0,6);
//        glEnable(GL_DEPTH_TEST);

        glClearColor(0.0f,0.0f,0.0f,0.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
        glm::mat4 view_matrix = glm::lookAt(eye_pos,eye_pos+camera_front*glm::vec3(3), camera_up);
        shaders.use();
        shaders.set_int("material.texture_diffuse1",0);
        shaders.set_mat4("model",e_matrix);
        shaders.set_mat4("view",view_matrix);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,depth_texture);
        glActiveTexture(GL_TEXTURE0);
//        glBindFramebuffer(GL_FRAMEBUFFER,depth_fbo);
//        depth_shader.use();
//        draw_func(depth_shader);
        draw_func(shaders);
//        light_source_shader.use();
//        light_source_shader.set_mat4("view",view_matrix);
//        glBindVertexArray(light_VAO);
//        glDrawArrays(GL_TRIANGLES,0,36);
        glfwSwapBuffers(window);
        glfwPollEvents();


    }
    
}