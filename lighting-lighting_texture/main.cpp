#include "../Shader.h"
#include "../variable.h"
#include "../util.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>

using namespace std;
#include "../util.h"




int main(){
    auto window = util::prepare_window();
    Shader s("D://Code/graphic/LearnOpenGL/lighting-lighting_texture/vertex.vs.glsl",
    "D://Code/graphic/LearnOpenGL/lighting-lighting_texture/fragment.fs.glsl");
    

    unsigned int VAO,VBO;
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO); //
    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(cube_vertices_with_normal_and_text_coords),cube_vertices_with_normal_and_text_coords,GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),0);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(sizeof(float)*3));
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(sizeof(float)*6));
    glEnableVertexAttribArray(0); // disable 
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);


    
    glm::vec3 eye_pos {0.f,0.f,5.f};
    glm::vec3 camera_up {0.f,1.f,0.f};
    glm::vec3 camera_front {0.f,0.f,1.f};
    glm::mat4 model_matrix(1.f);
    glm::vec3 light_pos{1.2f, 1.0f, 2.0f};



    model_matrix = glm::translate(model_matrix, {0.f,0.f,0.f});
    //model_matrix = glm::rotate(model_matrix,glm::radians(60.f),{1.f,0.f,0.f});
    
    int width = 800,height = 600;
    glm::mat4 projection_matrix = glm::perspective(45.f, static_cast<float>(width)/static_cast<float>(height), 0.1f, 100.f);
    glEnable(GL_DEPTH_TEST);
    s.use();
    glUniformMatrix4fv(glGetUniformLocation(s.program_id,"model"),1,GL_FALSE,glm::value_ptr(model_matrix));
    glUniformMatrix4fv(glGetUniformLocation(s.program_id,"projection"),1,GL_FALSE,glm::value_ptr(projection_matrix));
    s.set_vec3("object_color", {1.0f, 0.5f, 0.31f});
    s.set_vec3("light_color",{ 1.0f, 1.0f, 1.0f});
    s.set_vec3("material.specular",{0.50196078,0.50196078,0.50196078});
    s.set_float("material.shininess",32.f);
    s.set_float("material.diffuse",0);




    unsigned int text1;
    glGenTextures(1,&text1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,text1);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    int text1_width,text1_height,channels_num;
    unsigned char* text1_data = stbi_load("D://Code/graphic/LearnOpenGL/lighting-lighting_texture/container2.png",&text1_width,&text1_height,&channels_num,0);
    if(text1_data == nullptr){
        std::cout<<"load image fail"<<endl;
        return 0;
    }
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,text1_width,text1_height,0,GL_RGBA,GL_UNSIGNED_BYTE, text1_data);
    stbi_image_free(text1_data);
    s.set_int("material.diffuse",0);

    //specular texture
    unsigned int specular_text;
    glGenTextures(1, &specular_text);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,specular_text);
    int  specular_text_width,specular_text_height,specular_text_channel;
    auto specular_data =  stbi_load("D://Code/graphic/LearnOpenGL/lighting-lighting_texture/container2_specular.png",&specular_text_width,&specular_text_height,&specular_text_channel,0);
    if(specular_data == nullptr){
        cout<<"load specular texture failed"<<endl;
        return 0;
    }
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,specular_text_width,specular_text_height,0,GL_RGBA,GL_UNSIGNED_BYTE,specular_data);
    stbi_image_free(specular_data);
    glGenerateMipmap(GL_TEXTURE_2D);
    s.set_int("material.specular",1);

    unsigned int light_VAO;
    glGenVertexArrays(1, &light_VAO);
    glBindVertexArray(light_VAO);

    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),0);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(sizeof(float)*3));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    Shader light_source_shader("D://Code/graphic/LearnOpenGL/lighting-lighting_texture/vertex.vs.glsl",
    "D://Code/graphic/LearnOpenGL/lighting-lighting_texture/light_source.fs.glsl");
    light_source_shader.use();
    light_source_shader.set_mat4("projection",projection_matrix);
    light_source_shader.set_mat4("model", glm::scale(glm::translate(model_matrix, light_pos),glm::vec3(0.2f)) );
    light_source_shader.set_vec3("light_pos",light_pos);

    //light_source_shader.set_mat4("model", model_matrix);

    util::init_mouse(window, camera_front);
    while (!glfwWindowShouldClose(window)) {
        util::process_input(window,eye_pos,camera_front ,camera_up);

        //util::process_input(window, eye_pos, camera_front, camera_up);
        //cout<<eye_pos.x<<":"<<eye_pos.y<<":"<<eye_pos.z<<endl;

        //dynamic light source color by time
        glm::vec3 lightColor(1.0f,1.0f,1.0f);
//        lightColor.x = sin(glfwGetTime() * 2.0f);
//        lightColor.y = sin(glfwGetTime() * 0.7f);
//        lightColor.z = sin(glfwGetTime() * 1.3f);

        s.use();

        glm::mat4 view_matrix = glm::lookAt(eye_pos, eye_pos+camera_front*glm::vec3(3), {0.f,1.f,0.f});
        glUniformMatrix4fv(glGetUniformLocation(s.program_id,"view"),1,GL_FALSE,glm::value_ptr(view_matrix));
        s.set_vec3("light_pos",light_pos);
        s.set_vec3("view_pos", eye_pos);
//        s.set_vec3("light.ambient",  lightColor*0.2f); //less effect
//        s.set_vec3("light.diffuse",  lightColor*0.5f); //more effect 将光照调暗了一些以搭配场景
        s.set_vec3("light.ambient",  lightColor); //less effect
        s.set_vec3("light.diffuse",  lightColor); //more effect 将光照调暗了一些以搭配场景
        s.set_vec3("light.specular", {1.0f, 1.0f, 1.0f});

        light_source_shader.use();
        light_source_shader.set_mat4("view",view_matrix);
        light_source_shader.set_vec3("light_color",lightColor);

        //*************************

        s.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES,0,36);

        light_source_shader.use();
        glBindVertexArray(light_VAO);
        glDrawArrays(GL_TRIANGLES,0,36);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
        glClearColor(0.0f,0.0f,0.0f,0.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    }
    
}