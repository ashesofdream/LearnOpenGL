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
#include <random>


int main(){
    auto eye_pos = glm::vec3(0.f,0.f,125.f);
    auto camera_front = glm::vec3 (0.f,0.f,-1.f);
    auto camera_up = glm::vec3 (0.f,1.f,0.f);


    int small_plane_num = 100000;
    float circle_offset = 5.f;
    float radius = 50.f;

    std::random_device rand_dev;
    std::mt19937 gen(rand_dev());
    std::uniform_real_distribution<float>  dist(-circle_offset,circle_offset);
    std::uniform_real_distribution<float> scale_dist(0.05,0.25);
    std::uniform_real_distribution<float> angle_dist(0.f,360.f);
    auto window = util::prepare_window();
    auto projection_matrix = glm::perspective(glm::radians(45.f),800.f/600.f,1.f,200.f);
    //Shader
    Shader instance_shader("shaders/World.vs.glsl","shaders/World.fs.glsl");
    instance_shader.use();
    instance_shader.set_mat4("projection",projection_matrix);

    Shader planet_shader("shaders/big_planet.vs.glsl","shaders/World.fs.glsl");
    planet_shader.use();
    planet_shader.set_mat4("projection",projection_matrix);
    float planet_scale = 5.f;
    auto planet_model = glm::scale(glm::mat4(1.f),glm::vec3{planet_scale,planet_scale,planet_scale});
    planet_shader.set_mat4("model",planet_model);
    std::vector<glm::mat4> model_arrays(small_plane_num);
    for(int i = 0 ; i < small_plane_num ; ++i){
        glm::mat4  model(1.f);
        float angle = (float)i / (float)small_plane_num * 360.0f;
        float radians = glm::radians(angle);
        float displacement = dist(gen);
        float x = glm::sin(radians)*radius + displacement*15;
        displacement = dist(gen);
        float z = glm::cos(radians)*radius + displacement*20;
        displacement = dist(gen);
        float y = displacement * 0.2;
        model = glm::translate(model,{x,y,z});

        float scale = scale_dist(gen);
        model = glm::scale(model,{scale,scale,scale});

        float rotate_angle = angle_dist(gen);
        model = glm::rotate(model,glm::radians(rotate_angle),glm::vec3{0.4f,0.6f,0.8f});

        model_arrays[i] = model;
    }
    Model planet("../resource/planet.obj");
    Model rock("../resource/rock.obj");
    rock.instance_set_models(model_arrays);
    util::init_mouse(window,camera_front);
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window)){
        util::process_input(window,eye_pos,camera_front,camera_up);
        auto view_matrix =  glm::lookAt(eye_pos, eye_pos+camera_front, camera_up);
        instance_shader.use();
        instance_shader.set_mat4("view",view_matrix);
        rock.Draw(instance_shader,small_plane_num);
        planet_shader.use();
        planet_shader.set_mat4("view",view_matrix);
        planet.Draw(planet_shader);

        glfwSwapBuffers(window);
        glfwPollEvents();
        glClearColor(0.f,0.f,0.f,0.f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    }

}