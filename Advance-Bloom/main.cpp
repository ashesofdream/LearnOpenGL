
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "../Shader.h"
#include "../variable.h"
#include "../util.h"
#include "../Model.h"
#include "../Camera.h"

void DrawScene();
int main(){
    int scr_width = 800,scr_height = 600;
    glm::mat4 e_matrix(1.f);
    auto window = util::prepare_window();
    Shader split_shader("shaders/simple.vs.glsl","shaders/split_highlight.fs.glsl");
    Shader light_source_shader("shaders/simple.vs.glsl","shaders/light_source.fs.glsl");
    Shader gaussian_shader("../util/shaders/quad.vs.glsl","shaders/gaussian.fs.glsl");
    Shader blend_shader("../util/shaders/quad.vs.glsl","shaders/blend.fs.glsl");
    blend_shader.use();
    blend_shader.set_float("exposure", 1.f);
    blend_shader.set_int("origin", 0);
    blend_shader.set_int("gaussian", 1);
    //this part prepare for split shader
    GLuint couple_texts[2],couple_frame_buff;
    glGenTextures(2,couple_texts);
    glGenFramebuffers(1,&couple_frame_buff);
    glBindFramebuffer(GL_FRAMEBUFFER,couple_frame_buff);
    for(int i = 0; i < 2 ; ++i ){
        glBindTexture(GL_TEXTURE_2D,couple_texts[i]);
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB16F,scr_width,scr_height,0,GL_RGB,GL_FLOAT,nullptr);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(
            GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0+i,GL_TEXTURE_2D,couple_texts[i],0
        );
    }


    //this part prepare for gaussian
    GLuint gaussian_texts[2],gaussian_frame_buffs[2];
    glGenTextures(2, gaussian_texts);
    glGenFramebuffers(2,gaussian_frame_buffs);
    unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, attachments);
    for(int i= 0 ; i < 2 ; ++i){
        glBindFramebuffer(GL_FRAMEBUFFER,gaussian_frame_buffs[i]);
        glBindTexture(GL_TEXTURE_2D, gaussian_texts[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, scr_width, scr_height, 0, GL_RGB, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(
            GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,gaussian_texts[i],0
        );
    }
    // unsigned int rboDepth;
    // glGenRenderbuffers(1, &rboDepth);
    // glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    // glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, scr_width, scr_height);
    // glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
    // if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //prepare cube
    auto&& [cube_vbo,cube_vao] = util::GenVBOVAOAndBind();
    glBufferData(GL_ARRAY_BUFFER,sizeof(cube_vertices_with_normal_and_text_coords),cube_vertices_with_normal_and_text_coords,GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(float)*8,0);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,sizeof(float)*8,reinterpret_cast<void*>(sizeof(float)*6));
    glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,sizeof(float)*8,reinterpret_cast<void*>(sizeof(float)*3));
    
    std::vector<glm::vec3> lightPositions;
    lightPositions.push_back(glm::vec3( 0.0f, 0.5f,  1.5f));
    lightPositions.push_back(glm::vec3(-4.0f, 0.5f, -3.0f));
    lightPositions.push_back(glm::vec3( 3.0f, 0.5f,  1.0f));
    lightPositions.push_back(glm::vec3(-.8f,  2.4f, -1.0f));
    // colors
    std::vector<glm::vec3> lightColors;
    lightColors.push_back(glm::vec3(5.0f,   5.0f,  5.0f));
    lightColors.push_back(glm::vec3(10.0f,  0.0f,  0.0f));
    lightColors.push_back(glm::vec3(0.0f,   0.0f,  15.0f));
    lightColors.push_back(glm::vec3(0.0f,   5.0f,  0.0f));

    Camera camera(window);
    split_shader.use();
    split_shader.set_mat4("projection", camera.perspective_matrix());
    light_source_shader.use();
    light_source_shader.set_mat4("projection", camera.perspective_matrix());
    auto draw_scene = [&](Shader& s)->void{
        s.use();
        glBindFramebuffer(GL_FRAMEBUFFER,couple_frame_buff);
        glClearColor(0.f,0.f,0.f,0.0f);
        glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
        glBindVertexArray(cube_vao);
        s.set_mat4("view", camera.view_matrix());
        s.set_int("samp", 0);
        glActiveTexture(GL_TEXTURE0);
        //draw light_source     
        
        for(int i = 0 ; i < lightPositions.size();++i){
            glm::mat4 model_matrix = glm::translate(e_matrix,lightPositions[i]);
            s.set_mat4("model", model_matrix);
            s.set_mat4("normal_matrix", glm::transpose(glm::inverse(model_matrix)));
            s.set_vec3("light_color", lightColors[i]);
            
            glDrawArrays(GL_TRIANGLES,0,36);
        }
    };
    auto gaussian_draw = [&](Shader& s)->void{
        s.use();
        glActiveTexture(GL_TEXTURE0);
        GLuint draw_num = 10;
        GLboolean horizontal = true;
        s.set_int("samp", 0);
        for(GLuint cur_num = 0 ; cur_num < draw_num ; ++cur_num){
            glBindFramebuffer(GL_FRAMEBUFFER,gaussian_frame_buffs[horizontal]);
            glBindTexture(GL_TEXTURE_2D, cur_num == 0 ? couple_texts[1]:gaussian_texts[!horizontal]);
            s.set_int("horizontal", horizontal);
            util::draw_quad();
            horizontal = !horizontal;
        }
    };
    auto blend_draw = [&](Shader& s)->void{
        s.use();

        glBindFramebuffer(GL_FRAMEBUFFER,0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, couple_texts[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gaussian_texts[0]);
        
        util::draw_quad();
    };
    int i = 0;
    while(!glfwWindowShouldClose(window)){
        camera.update();
        glEnable(GL_DEPTH_TEST);
        //split_shader.use();
        draw_scene(light_source_shader);
        glDisable(GL_DEPTH_TEST);
        gaussian_draw(gaussian_shader);  
        blend_draw(blend_shader);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}