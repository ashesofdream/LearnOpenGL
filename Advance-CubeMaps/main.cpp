#include "../Shader.h"
#include "../variable.h"
#include "../util.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


int main(){
    auto window = util::prepare_window();
    auto scr_width = 800,scr_height = 600;
    glm::vec3 eye_pos{0.f,0.f,3.f};
    glm::vec3 eye_front{0.f,0.f,-1.f};
    glm::vec3 eye_top{0.f,1.f,0.f};

    auto projection_matrix = glm::perspective(glm::radians(45.f),static_cast<float>(scr_width)/scr_height,1.f,100.f);

    // auto sky_texture = util::loadCubemap(
    //     {"../resource/skybox/right.jpg",
    //     "../resource/skybox/left.jpg",
    //     "../resource/skybox/top.jpg",
    //     "../resource/skybox/bottom.jpg",
    //     "../resource/skybox/front.jpg",
    //     "../resource/skybox/back.jpg"}
    // );
    // // glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    // // glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    // // glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    // // glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    // // glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);
    // glBindTexture(GL_TEXTURE_CUBE_MAP,sky_texture);
    
    // Shader sky_shader("shaders/skybox.vs.glsl","shaders/skybox.fs.glsl");
    // glDepthMask(GL_FALSE);
    // sky_shader.use();
    
    // auto &&[skybox_vbo,skybox_vao]=util::GenVBOVAOAndBind();
    // glBufferData(GL_ARRAY_BUFFER,sizeof(skyboxVertices),skyboxVertices,GL_STATIC_DRAW);
    // glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),reinterpret_cast<void*>(0));
    // glEnableVertexAttribArray(0);
    // sky_shader.set_int("skybox", 0);
    // sky_shader.set_mat4("projection", projection_matrix);
    // util::init_mouse(window, eye_front);
    
    
    //a sample box;
    auto&& [box_vbo,box_vao] = util::GenVBOVAOAndBind();
    glBufferData(GL_ARRAY_BUFFER,sizeof(cubeVertices_with_texture),cubeVertices_with_texture,GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(float)*5,reinterpret_cast<void*>(0));
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,sizeof(float)*5,reinterpret_cast<void*>(sizeof(float)*3));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    auto box_texture = util::texture_from_file("container.jpg", "../resource/");
    Shader world_shader("shaders/world.vs.glsl","shaders/world.fs.glsl");
    world_shader.use();
    world_shader.set_int("samp", 0);
    world_shader.set_mat4("model", glm::mat4(1.0f));
    world_shader.set_mat4("projection", projection_matrix);

    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window)) {
        util::process_input(window, eye_pos, eye_front, eye_top);
        // sky_shader.use();
        // glDepthMask(GL_FALSE);
        // glBindVertexArray(skybox_vao);
        // glBindTexture(GL_TEXTURE_CUBE_MAP,sky_texture);
        
        auto view_matrix = glm::lookAt(eye_pos,eye_pos+eye_front*glm::vec3(3.f),eye_top);
        // sky_shader.set_mat4("view", view_matrix);
        // glDrawArrays(GL_TRIANGLES,0,36);
        // glDepthMask(GL_TRUE);

        world_shader.use();
        world_shader.set_mat4("view", view_matrix);
        glBindTexture(GL_TEXTURE_2D,box_texture);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(box_vao);
        glDrawArrays(GL_TRIANGLES,0,36);


        glClearColor(0.1f,0.1f,0.1f,1.f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();

    }

}