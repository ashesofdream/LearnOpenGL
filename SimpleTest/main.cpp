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


int main(){
    auto window = util::prepare_window();
    glm::vec3 eye_pos {0.f,0.f,5.f};
    glm::vec3 camera_up {0.f,1.f,0.f};
    glm::vec3 camera_front {0.f,0.f,-1.f};


    Shader worldShader("shaders/reflect_box.vs.glsl",
    "shaders/reflect_box.fs.glsl");
    worldShader.use();
    //load robot model
    Model robot_model("../resource/nanosuit.obj");

    util::init_mouse(window, camera_front);
    glEnable(GL_DEPTH_TEST);
    Model m ("../resource/nanosuit.obj");
    while(!glfwWindowShouldClose(window)){
       util::process_input(window, eye_pos, camera_front, camera_up);
        glClearColor(0.0f,0.0f,0.0f,0.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

        worldShader.use();
        worldShader.set_mat4("model",glm::scale(glm::mat4(1.f), {0.5f,0.5f,0.5f}));
        m.Draw(worldShader);

        glfwSwapBuffers(window);
        glfwPollEvents();

    }
}