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
void RenderQuad();
int main(){
    
    int scr_w = 800,scr_h = 600;
    glm::vec3 light_pos(0.5f, 1.0f, 0.3f);;
    glm::vec3 camera_front{0.f,0.f,-1.f};
    glm::vec3 camera_up{0.f,1.f,0.f};
    glm::vec3 view_pos{0.f,0.f,1.f};
    

    const auto& window = util::prepare_window();
    
    auto&& [brick_vao,vbo] = util::GenVBOVAOAndBind();
    auto wall_text =util::texture_from_file("bricks2.jpg", "../resource/");
    auto wall_normal_text = util::texture_from_file("bricks2_normal.jpg", "../resource/");
    auto wall_disp_text = util::texture_from_file("bricks2_disp.jpg", "../resource/");
    

    Shader s("shaders/brick.vs.glsl","shaders/brick.fs.glsl");
    s.use();
    auto projection_matrix= glm::perspective(glm::radians(45.f),static_cast<float>(scr_w)/scr_h,0.1f,100.f);
    s.set_mat4("projection", projection_matrix);
    s.set_vec3("light_pos", light_pos);
    s.set_int("normal_map", 1);
    s.set_int("texture_map", 0);
    s.set_int("parallax_map",2);
    s.set_float("height_scale", 0.2f);

    util::init_mouse(window, camera_front);
    while (!glfwWindowShouldClose(window)) {
        util::process_input(window, view_pos, camera_front, camera_up);
        s.set_mat4("model", glm::mat4(1.f));
        s.set_mat4("view", glm::lookAt(view_pos, view_pos+camera_front*glm::vec3(3.f), camera_up));
        s.set_vec3("view_pos", view_pos);
        
        glClearColor(0.f,0.f,0.f,0.f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        s.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,wall_text);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,wall_normal_text);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D,wall_disp_text);
        RenderQuad();
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

// void RenderQuad(){
//     static GLuint quadVAO = 0,quadVBO=0;
//     if(quadVAO==0){
//         // positions
//         glm::vec3 pos1(-1.0, 1.0, 0.0);
//         glm::vec3 pos2(-1.0, -1.0, 0.0);
//         glm::vec3 pos3(1.0, -1.0, 0.0);
//         glm::vec3 pos4(1.0, 1.0, 0.0);
//         // texture coordinates
//         glm::vec2 uv1(0.0, 1.0);
//         glm::vec2 uv2(0.0, 0.0);
//         glm::vec2 uv3(1.0, 0.0);
//         glm::vec2 uv4(1.0, 1.0);
//         // normal vector
//         glm::vec3 nm(0.0, 0.0, 1.0);

//         // calculate tangent/bitangent vectors of both triangles
//         glm::vec3 tangent1, bitangent1;
//         glm::vec3 tangent2, bitangent2;
    
//         auto delta_uv_1_2 = uv2-uv1;
//         auto delta_uv_1_3 = uv3-uv1;
//         auto ret = glm::transpose(1/(delta_uv_1_2.x*delta_uv_1_3.y-delta_uv_1_2.y*delta_uv_1_3.x) *
//         glm::mat2(glm::vec2(delta_uv_1_3.y,-delta_uv_1_3.x),glm::vec2(-delta_uv_1_2.y,delta_uv_1_2.x)) *
//          glm::transpose(glm::mat<2,3,float>(pos2-pos1,pos3-pos1)));
//         tangent1 = glm::normalize(ret[0]);bitangent1 = glm::normalize(ret[1]) ;

//         auto delta_uv_1_4 = uv4-uv1;
//         ret = glm::transpose(1/(delta_uv_1_4.x*delta_uv_1_3.y-delta_uv_1_4.y*delta_uv_1_3.x) *
//         glm::mat2(delta_uv_1_4.y,-delta_uv_1_3.y,-delta_uv_1_4.x,delta_uv_1_3.x) *
//          glm::transpose(glm::mat<2,3,float>(pos4-pos1,pos3-pos1)));
//         ret = glm::transpose(1/(delta_uv_1_3.x*delta_uv_1_4.y - delta_uv_1_3.y*delta_uv_1_4.x)*
//         glm::mat2(glm::vec2(delta_uv_1_4.y,-delta_uv_1_4.x),glm::vec2(-delta_uv_1_3.y,delta_uv_1_3.x)) *
//         glm::transpose(glm::mat<2, 3, float>(pos3-pos1,pos4-pos1)));
//         tangent2 = glm::normalize(ret[0]);bitangent2 = glm::normalize(ret[1]);

//         GLfloat quadVertices[] = {
//             // Positions            // normal         // TexCoords  // Tangent                          // Bitangent
//             pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
//             pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
//             pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

//             pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
//             pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
//             pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
//         };
//         glGenVertexArrays(1, &quadVAO);
//         glGenBuffers(1, &quadVBO);
//         glBindVertexArray(quadVAO);
//         glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
//         glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
//         glEnableVertexAttribArray(0);
//         glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)0);
//         glEnableVertexAttribArray(1);
//         glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
//         glEnableVertexAttribArray(2);
//         glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
//         glEnableVertexAttribArray(3);
//         glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));
//         glEnableVertexAttribArray(4);
//         glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(11 * sizeof(GLfloat)));
//     }
//     glBindVertexArray(quadVAO);
//     glDrawArrays(GL_TRIANGLES, 0, 6);
//     glBindVertexArray(0);

// }
GLuint quadVAO = 0;
GLuint quadVBO;
void RenderQuad()
{
    if (quadVAO == 0)
    {
        // positions
        glm::vec3 pos1(-1.0, 1.0, 0.0);
        glm::vec3 pos2(-1.0, -1.0, 0.0);
        glm::vec3 pos3(1.0, -1.0, 0.0);
        glm::vec3 pos4(1.0, 1.0, 0.0);
        // texture coordinates
        glm::vec2 uv1(0.0, 1.0);
        glm::vec2 uv2(0.0, 0.0);
        glm::vec2 uv3(1.0, 0.0);
        glm::vec2 uv4(1.0, 1.0);
        // normal vector
        glm::vec3 nm(0.0, 0.0, 1.0);

        // calculate tangent/bitangent vectors of both triangles
        glm::vec3 tangent1, bitangent1;
        glm::vec3 tangent2, bitangent2;
        // - triangle 1
        glm::vec3 edge1 = pos2 - pos1;
        glm::vec3 edge2 = pos3 - pos1;
        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;

        GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
        tangent1 = glm::normalize(tangent1);

        bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
        bitangent1 = glm::normalize(bitangent1);

        // - triangle 2
        edge1 = pos3 - pos1;
        edge2 = pos4 - pos1;
        deltaUV1 = uv3 - uv1;
        deltaUV2 = uv4 - uv1;

        f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
        tangent2 = glm::normalize(tangent2);


        bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
        bitangent2 = glm::normalize(bitangent2);


        GLfloat quadVertices[] = {
            // Positions            // normal         // TexCoords  // Tangent                          // Bitangent
            pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
            pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
            pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

            pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
            pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
            pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
        };
        // Setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(11 * sizeof(GLfloat)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}