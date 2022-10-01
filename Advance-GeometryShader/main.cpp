//
// Created by GXDC on 2022/9/29.
//

#include "../Shader.h"
#include "../variable.h"
#include "../util.h"
#include "../Model.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


int main(){
    auto eye_pos = glm::vec3(0.f,0.f,5.f);
    auto camera_front = glm::vec3 (0.f,0.f,-1.f);
    auto camera_up = glm::vec3 (0.f,1.f,0.f);

    auto window = util::prepare_window();
    Shader cur_shader("shaders/World.vs.glsl","shaders/fragment_shader.fs.glsl","shaders/World.gs.glsl");
    cur_shader.use();



    unsigned int uniform_buffer_obj;
    glGenBuffers(1,&uniform_buffer_obj);
    glBindBuffer(GL_UNIFORM_BUFFER,uniform_buffer_obj);
    glBindBufferBase(GL_UNIFORM_BUFFER,0,uniform_buffer_obj);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4)*2, nullptr,GL_STATIC_DRAW);
    auto projection_matrix = glm::perspective(glm::radians(45.f),800.f/600.f,0.1f,100.f);
    glBufferSubData(GL_UNIFORM_BUFFER,0, sizeof(glm::mat4), glm::value_ptr(projection_matrix));
    util::init_mouse(window,camera_front);
    Model robot_model("../resource/nanosuit.obj");
//    cur_shader.set_mat4("model",glm::mat4(1.f));
    glEnable(GL_DEPTH_TEST);

    Shader model_shader("shaders/World.vs.glsl","shaders/model.fs.glsl");
    model_shader.use();
    model_shader.set_vec3("light.light_dir", glm::normalize(glm::vec3{1.f,1.f,1.f}));
    model_shader.set_vec3("light.ambient", {0.2f, 0.2f, 0.2f}); //less effect
    model_shader.set_vec3("light.diffuse", {0.5f, 0.5f, 0.5f}); //more effect 将光照调暗了一些以搭配场景
    model_shader.set_vec3("light.specular", {1.0f, 1.0f, 1.0f});

    while (!glfwWindowShouldClose(window)){
        util::process_input(window,eye_pos,camera_front,camera_up);
        auto view_matrix = glm::lookAt(eye_pos,eye_pos+camera_front,camera_up);
        glBindBuffer(GL_UNIFORM_BUFFER,uniform_buffer_obj);
        glBufferSubData(GL_UNIFORM_BUFFER,sizeof(glm::mat4),sizeof(glm::mat4),glm::value_ptr(view_matrix));

        cur_shader.use();
        cur_shader.set_mat4("model",glm::scale(glm::mat4(1.f),glm::vec3(0.3f,0.3f,0.3f)));

        robot_model.Draw(cur_shader);

        model_shader.use();
        model_shader.set_mat4("model",glm::scale(glm::mat4(1.f),glm::vec3(0.3f,0.3f,0.3f)));
        model_shader.set_vec3("eye_pos",eye_pos);
        robot_model.Draw(model_shader);

        glfwSwapBuffers(window);
        glfwPollEvents();
        glClearColor(0.f,0.f,0.f,0.f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    }

}