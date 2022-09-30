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
    auto window = util::prepare_window();
    auto &&[points_vbo,points_vao] = util::GenVBOVAOAndBind();
    float triangles[] = {-0.5f,-0.5f,0.f,
                         0.5f,-0.5f,0.f,
                            0.5f,0.5f,0.f};
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangles),triangles,GL_STATIC_DRAW);
    glad_glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, sizeof(float)*3, nullptr);
    glEnableVertexAttribArray(0);
    Shader cur_shader("shaders/vertex_shader.vs.glsl","shaders/fragment_shader.fs.glsl","shaders/geometry_shader.gs.glsl");
    cur_shader.use();

    while (!glfwWindowShouldClose(window)){
        cur_shader.use();
        glBindVertexArray(points_vao);
        glDrawArrays(GL_POINTS,0,3);

        glfwSwapBuffers(window);
        glfwPollEvents();
        glClearColor(0.f,0.f,0.f,0.f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

}