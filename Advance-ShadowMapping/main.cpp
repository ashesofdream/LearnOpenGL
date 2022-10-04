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


using namespace std;




int main(){
    float scr_width = 800,scr_height = 600;
    auto window = util::prepare_window();
    Shader shaders("shaders/vertex.vs.glsl",
    "shaders/fragment.fs.glsl");

    glm::vec3 eye_pos (-2.f,4.f,-1.f);
    glm::vec3 camera_up {0.f,1.f,0.f};
    glm::vec3 camera_front {0.f,0.f,1.f};
    glm::vec3 light_pos{1.2f, 1.0f, 2.0f};
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

    unsigned int cubeVAO,cubeVBO;
    glGenBuffers(1,&cubeVBO);
    glGenVertexArrays(1,&cubeVAO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER,cubeVBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(cubeVertices_with_texture),&cubeVertices_with_texture,GL_STATIC_DRAW);
    
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(float)*5,nullptr);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(float)*5,(void*)(sizeof(float)*3));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(2);

    auto&& [grass_VBO,grass_VAO] = util::GenVBOVAOAndBind();
    glBufferData(GL_ARRAY_BUFFER,sizeof(transparentVertices_with_texture),transparentVertices_with_texture,GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(float)*5,nullptr);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(float)*5,(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(2);
    

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

    
    
    unsigned int ground_text_id = util::texture_from_file("metal.png","../resource/");
    unsigned int cube_text_id = util::texture_from_file("marble.jpg", "../resource/");
    unsigned int grass_text_id = util::texture_from_file("grass.png", "../resource/");
    unsigned  int window_text_id = util::texture_from_file("blending_transparent_window.png","../resource");
    //mvp
    glm::mat4 projection_matrix = glm::perspective(glm::radians(45.f),scr_width/scr_height,1.f,100.f);
    shaders.set_mat4("projection",projection_matrix);
    glm::mat4 e_matrix = glm::mat4 (1.f);

    //model_matrix = glm::rotate(model_matrix,glm::radians(60.f),{1.f,0.f,0.f});


    //light_source
    unsigned int light_VAO,VBO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &light_VAO);
    glBindVertexArray(light_VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(cube_vertices),cube_vertices,GL_STATIC_DRAW);


    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    Shader light_source_shader("shaders/vertex.vs.glsl",
                               "shaders/light_source.fs.glsl");

    light_source_shader.use();
    light_source_shader.set_mat4("projection",projection_matrix);
    light_source_shader.set_mat4("model", glm::scale(glm::translate(glm::mat4 (1.0f), light_pos),glm::vec3(0.2f)) );
    light_source_shader.set_vec3("light_pos",light_pos);


    glEnable(GL_DEPTH_TEST);
    util::init_mouse(window,camera_front);

    vector<glm::vec3> vegetation;
    vegetation.push_back(glm::vec3(-1.5f,  0.0f, -0.48f));
    vegetation.push_back(glm::vec3( 1.5f,  0.0f,  0.51f));
    vegetation.push_back(glm::vec3( 0.0f,  0.0f,  0.7f));
    vegetation.push_back(glm::vec3(-0.3f,  0.0f, -2.3f));
    vegetation.push_back(glm::vec3( 0.5f,  0.0f, -0.6f));

    vector<glm::vec3> windows_pos
            {
                    glm::vec3(-1.5f, 0.0f, -0.48f),
                    glm::vec3( 1.5f, 0.0f, 0.51f),
                    glm::vec3( 0.0f, 0.0f, 0.7f),
                    glm::vec3(-0.3f, 0.0f, -2.3f),
                    glm::vec3( 0.5f, 0.0f, -0.6f)
            };
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    //from here is shadow mapping content
    unsigned int depth_texture;
    glGenTextures(1, &depth_texture);
    glBindTexture(GL_TEXTURE_2D, depth_texture);
    glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,scr_width,scr_height,0,GL_DEPTH_COMPONENT,GL_FLOAT,NULL);

    unsigned int depth_fbo;
    glGenFramebuffers(1, &depth_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, depth_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,depth_texture,0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER,0);

    //light space
    auto light_space_projection_matrix = glm::ortho(-10.f,10.f,-10.f,10.f,0.1f,10.f);
    auto light_space_view_matrix = glm::lookAt(glm::vec3(0.f,1.f,2.f),glm::vec3 (0.f,0.f,0.f),glm::vec3 (0.f,1.f,0.f));
    Shader depth_shader("shaders/depth_shader.vs.glsl","shaders/depth_shader.fs.glsl");
    depth_shader.use();
    depth_shader.set_mat4("projection",light_space_projection_matrix);
    depth_shader.set_mat4("view",light_space_view_matrix);

    //show shader texture;
    Shader direct_shader("shaders/direct_buffer_out.vs.glsl","shaders/direct_buffer_out.fs.glsl");
    float show_texture_array[] = {-1.f,1.f,0.f,1.f,
                                  1.f, 1.f,1.f,1.f,
                                   -1.f,-1.f,0.f,0.f,

                                   -1.f,-1.f,0.f,0.f,
                                   1.f,-1.f,1.f,0.f,
                                   1.f,1.f,1.f,1.f};
//    float show_texture_array[] ={ // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
//            // positions   // texCoords
//            -1.0f,  1.0f,  0.0f, 1.0f,
//            -1.0f, -1.0f,  0.0f, 0.0f,
//            1.0f, -1.0f,  1.0f, 0.0f,
//
//            -1.0f,  1.0f,  0.0f, 1.0f,
//            1.0f, -1.0f,  1.0f, 0.0f,
//            1.0f,  1.0f,  1.0f, 1.0f
//    };
    auto&&[show_texture_vbo,show_texture_vao] = util::GenVBOVAOAndBind();
    glBufferData(GL_ARRAY_BUFFER, sizeof(show_texture_array),show_texture_array,GL_STATIC_DRAW);
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,sizeof(float)*4,nullptr);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,sizeof(float)*4,reinterpret_cast<void*>(sizeof (float)*2));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    direct_shader.set_int("samp",0);

    auto draw_func = [&](Shader& shader_){
        shader_.set_mat4("model", glm::mat4(1.0f));
        glBindVertexArray(GroundVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,ground_text_id);
        glDrawArrays(GL_TRIANGLES,0,6);

//        glBindVertexArray(cubeVAO);
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D,cube_text_id);
//        glDrawArrays(GL_TRIANGLES,0,36);

//        glm::mat4 pos_matrix = glm::translate(e_matrix, glm::vec3(-1.0f, 0.0f, -1.0f));
//        shader_.set_mat4("model", pos_matrix);
//        pos_matrix = glm::translate(e_matrix, glm::vec3(2.0f, 0.0f, 0.0f));
//        shader_.set_mat4("model", pos_matrix);
//        glDrawArrays(GL_TRIANGLES,0,36);
//
//        glBindVertexArray(grass_VAO);
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D,grass_text_id);
//        for(const auto& pos : vegetation){
//            shader_.set_mat4("model", glm::translate(e_matrix,pos) );
//            glDrawArrays(GL_TRIANGLES,0,6);
//        };
//
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D,window_text_id);
//        for(const auto& window_pos : windows_pos) {
//            shader_.set_mat4("model", glm::translate(e_matrix,window_pos));
//            glDrawArrays(GL_TRIANGLES, 0, 6);
//        }
    };
    while (!glfwWindowShouldClose(window)) {
        util::process_input(window,eye_pos,camera_front ,camera_up);

//        glBindFramebuffer(GL_FRAMEBUFFER,depth_fbo);
//        depth_shader.use();
//        draw_func(depth_shader);
//
//        glBindFramebuffer(GL_FRAMEBUFFER,0);
//        direct_shader.use();
//        glDisable(GL_DEPTH_TEST);
//        glBindVertexArray(show_texture_vao);
//        glBindTexture(GL_TEXTURE_2D,depth_texture);
//        glActiveTexture(GL_TEXTURE0);
//        glDrawArrays(GL_TRIANGLES,0,6);
//        glEnable(GL_DEPTH_TEST);

        glm::mat4 view_matrix = glm::lookAt(eye_pos,eye_pos+camera_front*glm::vec3(3), camera_up);
        shaders.use();
        shaders.set_int("material.diffuse",0);
        shaders.set_mat4("model",e_matrix);
        shaders.set_mat4("view",view_matrix);

        draw_func(shaders);
//        light_source_shader.use();
//        light_source_shader.set_mat4("view",view_matrix);
//        glBindVertexArray(light_VAO);
//        glDrawArrays(GL_TRIANGLES,0,36);
        glfwSwapBuffers(window);
        glfwPollEvents();
        glClearColor(0.0f,0.0f,0.0f,0.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
    }
    
}