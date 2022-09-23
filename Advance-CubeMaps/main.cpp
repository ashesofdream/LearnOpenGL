#include "../Shader.h"
#include "../variable.h"
#include "../util.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


int main(){
    auto window = util::prepare_window();
    auto texture_id = util::loadCubemap(
        {"../resource/skybox/right.jpg",
        "../resource/skybox/left.jpg",
        "../resource/skybox/top.jpg",
        "../resource/skybox/bottom.jpg",
        "../resource/skybox/front.jpg",
        "../resource/skybox/back.jpg"}
    );
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP,texture_id);
    
    Shader sky_shader("shaders/skybox.vs.glsl","shaders/skybox.fs.glsl");
    glDepthMask(GL_FALSE);
    sky_shader.use();
    
    auto &&[skybox_vbo,skybox_vao]=util::GenVBOVAOAndBind();
    glBufferData(GL_ARRAY_BUFFER,sizeof(skyboxVertices),skyboxVertices,GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);
    sky_shader.set_int("skybox", 0);
    while (!glfwWindowShouldClose(window)) {
        glBindVertexArray(skybox_vao);
        glBindTexture(GL_TEXTURE_CUBE_MAP,texture_id);
        glDrawArrays(GL_TRIANGLES,0,36);

    }

}