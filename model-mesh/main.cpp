#include "../Shader.h"
#include "../variable.h"
#include "../util.h"
#include "../Model.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

using namespace std;


int main(){
    int scr_width=800, scr_height=600;
    auto window = util::prepare_window();
    glEnable(GL_DEPTH_TEST);
    Shader s("shaders/vertex.vs.glsl",
    "shaders/fragment.fs.glsl");

    glm::vec3 eye_pos {0.f,0.f,5.f};
    glm::vec3 camera_up {0.f,1.f,0.f};
    glm::vec3 camera_front {0.f,0.f,1.f};
    glm::vec3 light_pos{1.2f, 1.0f, 2.0f};

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    Model model("../resource/nanosuit.obj");


    s.use();
    glm::mat4 projection_matrix = glm::perspective(45.f, static_cast<float>(scr_width)/static_cast<float>(scr_height), 0.1f, 100.f);
    s.set_mat4("projection",projection_matrix);
     //set spotlight
    s.set_vec3("spotLight.ambient", {0.2f, 0.2f, 0.2f}); //less effect
    s.set_vec3("spotLight.diffuse", {0.5f, 0.5f, 0.5f}); //more effect 将光照调暗了一些以搭配场景
    s.set_vec3("spotLight.specular", {1.0f, 1.0f, 1.0f});
    s.set_vec3("spotLight.spot_dir",camera_front);
    s.set_float("spotLight.cos_inner_cut_off",glm::cos(glm::radians(12.5f)));
    s.set_float("spotLight.cos_outer_cut_off",glm::cos(glm::radians(17.5f)));
    s.set_float("spotLight.constant",1.0f);
    s.set_float("spotLight.linear",0.09f);
    s.set_float("spotLight.quadratic",0.032f);

    //参照物
    unsigned int light_VAO,VBO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &light_VAO);

    glBindVertexArray(light_VAO);

    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),0);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(sizeof(float)*3));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    Shader light_source_shader("vertex.vs.glsl",
                               "light_source.fs.glsl");
    light_source_shader.use();
    light_source_shader.set_mat4("projection",projection_matrix);
    light_source_shader.set_mat4("model", glm::scale(glm::translate(glm::mat4 (1.0f), light_pos),glm::vec3(0.2f)) );
    light_source_shader.set_vec3("light_pos",light_pos);

    

    util::init_mouse(window, camera_front);
    while (!glfwWindowShouldClose(window)) {
        util::process_input(window,eye_pos,camera_front ,camera_up);
        s.use();
        s.set_mat4("model",glm::scale(glm::mat4(1.0f),glm::vec3(0.3f,0.3f,0.3f)));
        s.set_vec3("spotLight.spot_dir",camera_front);
        s.set_vec3("spotLight.position",eye_pos);
        glm::mat4 view_matrix = glm::lookAt(eye_pos,eye_pos+glm::vec3(3)*camera_front,camera_up);
        s.set_mat4("view",view_matrix);
        model.Draw(s);

        light_source_shader.use();
        light_source_shader.set_mat4("view",view_matrix);
        glfwSwapBuffers(window);
        glfwPollEvents();
        glClearColor(0.0f,0.0f,0.0f,0.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    }
    
}