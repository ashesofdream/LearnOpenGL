#include "../Shader.h"
#include "../variable.h"
#include "../util.h"
#include "../Model.h"
#include "../Camera.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int main(){
    int scr_width = 800,scr_height = 600;
    auto window = util::prepare_window();
    GLuint couple_texts[2],couple_frame_buff[2];
    glGenTextures(2,couple_texts);
    glGenFramebuffers(2,couple_frame_buff);
    for(int i = 0; i < 2 ; ++i ){
        glBindFramebuffer(GL_TEXTURE_2D,couple_frame_buff[i]);
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
    Camera camera(window);
    while(!glfwWindowShouldClose(window)){
        camera.update();
        
    }
}