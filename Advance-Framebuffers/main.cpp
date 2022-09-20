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
#include<unordered_map>
#include<deque>
#include <set>
#include <map>


using namespace std;




int main(){
    float scr_width = 800,scr_height = 600;
    auto window = util::prepare_window();
    Shader shaders("shaders/vertex.vs.glsl",
    "shaders/fragment.fs.glsl");

    string_view  view;

    using iter_type =    unordered_map<string,int>::iterator;
    auto func = [](const iter_type& left,const iter_type& right){
        return left->first.size() < right->first.size()||left->first<right->first;
    };




    glm::vec3 eye_pos {0.f,0.f,5.f};
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

    //annother framebuffer
    unsigned int fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER,fbo);

    //bind texture attachment
    unsigned int texture_attach;
    glGenTextures(1, &texture_attach);
    glBindTexture(GL_TEXTURE_2D,texture_attach);
    glTexImage2D(GL_TEXTURE_2D, 0 ,GL_RGB,scr_height,scr_width,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D,0);

    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,texture_attach,0);

    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER,rbo);
    glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH24_STENCIL8,scr_height,scr_width);
    glBindRenderbuffer(GL_RENDERBUFFER,0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_RENDERBUFFER,rbo);
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    map<int,int> m;
    m.insert(make_pair(3,3));
    

    while (!glfwWindowShouldClose(window)) {
        util::process_input(window,eye_pos,camera_front ,camera_up);

        glm::mat4 view_matrix = glm::lookAt(eye_pos,eye_pos+camera_front*glm::vec3(3), camera_up);
        shaders.use();
        shaders.set_int("material.diffuse",0);
        shaders.set_mat4("model",e_matrix);
        shaders.set_mat4("view",view_matrix);
        glBindVertexArray(GroundVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,ground_text_id);
        glDrawArrays(GL_TRIANGLES,0,6);

        glBindVertexArray(cubeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,cube_text_id);
        glm::mat4 pos_matrix = glm::translate(e_matrix, glm::vec3(-1.0f, 0.0f, -1.0f));
        shaders.set_mat4("model", pos_matrix);

        glDrawArrays(GL_TRIANGLES,0,36);
        
        pos_matrix = glm::translate(e_matrix, glm::vec3(2.0f, 0.0f, 0.0f));
        shaders.set_mat4("model", pos_matrix);
        glDrawArrays(GL_TRIANGLES,0,36);

        glBindVertexArray(grass_VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,grass_text_id);
        for(const auto& pos : vegetation){
            shaders.set_mat4("model", glm::translate(e_matrix,pos) );
            glDrawArrays(GL_TRIANGLES,0,6);
        };

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,window_text_id);
        for(const auto& window_pos : windows_pos) {
            shaders.set_mat4("model", glm::translate(e_matrix,window_pos));
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        light_source_shader.use();
        light_source_shader.set_mat4("view",view_matrix);
        glBindVertexArray(light_VAO);
        glDrawArrays(GL_TRIANGLES,0,36);
        glfwSwapBuffers(window);
        glfwPollEvents();
        glClearColor(0.0f,0.0f,0.0f,0.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
    }
    
}