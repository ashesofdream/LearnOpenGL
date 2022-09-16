#include "../Shader.h"
#include "../variable.h"
#include "../util.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>
#include <iostream>


using namespace std;
#include "../util.h"




int main(){
    auto window = util::prepare_window();
    Shader s("shaders/vertex.vs.glsl",
    "shaders/fragment.fs.glsl");
    glm::vec3 pointLightPositions[] = {
            glm::vec3( 0.7f,  0.2f,  2.0f),
            glm::vec3( 2.3f, -3.3f, -4.0f),
            glm::vec3(-4.0f,  2.0f, -12.0f),
            glm::vec3( 0.0f,  0.0f, -3.0f)
    };

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
    glm::vec3 light_pos{1.2f, 1.0f, 2.0f};



    //model_matrix = glm::rotate(model_matrix,glm::radians(60.f),{1.f,0.f,0.f});
    
    int width = 800,height = 600;
    glm::mat4 projection_matrix = glm::perspective(45.f, static_cast<float>(width)/static_cast<float>(height), 0.1f, 100.f);

    s.use();
    glUniformMatrix4fv(glGetUniformLocation(s.program_id,"projection"),1,GL_FALSE,glm::value_ptr(projection_matrix));
    s.set_vec3("light_color",{ 1.0f, 1.0f, 1.0f});

    s.set_float("material.shininess",32.f);
    s.set_float("material.diffuse",0);
    s.set_float("material.specular",0);

    unsigned int text1;
    glGenTextures(1,&text1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,text1);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    int text1_width,text1_height,channels_num;
    unsigned char* text1_data = stbi_load("D://Code/graphic/LearnOpenGL/Advance-stencil_testing/container2.png",&text1_width,&text1_height,&channels_num,0);
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
    auto specular_data =  stbi_load("D://Code/graphic/LearnOpenGL/Advance-stencil_testing/container2_specular.png",&specular_text_width,&specular_text_height,&specular_text_channel,0);
    if(specular_data == nullptr){
        cout<<"load specular texture failed"<<endl;
        return 0;
    }
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,specular_text_width,specular_text_height,0,GL_RGBA,GL_UNSIGNED_BYTE,specular_data);
    stbi_image_free(specular_data);
    glGenerateMipmap(GL_TEXTURE_2D);
    s.set_int("material.specular",1);
    s.set_float("spotLight.constant",1.0f);
    s.set_float("spotLight.linear",0.09f);
    s.set_float("spotLight.quadratic",0.032f);
    int point_light_num = 4;
    s.set_arrays_vec3(point_light_num,"pointLights","ambient", {0.05f, 0.05f, 0.05f});
    s.set_arrays_vec3(point_light_num,"pointLights","diffuse",{0.8f, 0.8f, 0.8f});
    s.set_arrays_vec3(point_light_num,"pointLights","specular",{1.f,1.f,1.f});
    s.set_arrays_float(point_light_num,"pointLights","constant",1.f);
    s.set_arrays_float(point_light_num,"pointLights","linear",0.09f);
    s.set_arrays_float(point_light_num,"pointLights","quadratic",0.032f);
    for(int i = 0 ; i < point_light_num; ++i){
        s.set_vec3(("pointLights["s+ to_string(i)+"].position").c_str(),pointLightPositions[i]);
    }
    cout<<"";
    unsigned int light_VAO;
    glGenVertexArrays(1, &light_VAO);
    glBindVertexArray(light_VAO);

    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),0);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(sizeof(float)*3));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    Shader light_source_shader("shaders/vertex.vs.glsl",
    "shaders/light_source.fs.glsl");
    light_source_shader.use();
    light_source_shader.set_mat4("projection",projection_matrix);
    light_source_shader.set_mat4("model", glm::scale(glm::translate(glm::mat4 (1.0f), light_pos),glm::vec3(0.2f)) );
    light_source_shader.set_vec3("light_pos",light_pos);

    unsigned int border_VAO;
    glGenVertexArrays(1,&border_VAO);
    glBindVertexArray(border_VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),0);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(sizeof(float)*3));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    Shader border_shader("shaders/vertex.vs.glsl","shaders/light_source.fs.glsl");
    border_shader.use();
    border_shader.set_mat4("projection",projection_matrix);
    border_shader.set_vec3("light_pos",light_pos);


    //light_source_shader.set_mat4("model", model_matrix);

    util::init_mouse(window, camera_front);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    while (!glfwWindowShouldClose(window)) {
        util::process_input(window,eye_pos,camera_front ,camera_up);

        glm::vec3 lightColor(1.0f,1.0f,1.0f);


        s.use();

        glm::mat4 view_matrix = glm::lookAt(eye_pos, eye_pos+camera_front*glm::vec3(3), {0.f,1.f,0.f});
        glUniformMatrix4fv(glGetUniformLocation(s.program_id,"view"),1,GL_FALSE,glm::value_ptr(view_matrix));
        s.set_vec3("light_pos",light_pos);
        s.set_vec3("view_pos", eye_pos);

        s.set_vec3("spotLight.ambient", {0.2f, 0.2f, 0.2f}); //less effect
        s.set_vec3("spotLight.diffuse", {0.5f, 0.5f, 0.5f}); //more effect 将光照调暗了一些以搭配场景
        s.set_vec3("spotLight.specular", {1.0f, 1.0f, 1.0f});
        s.set_vec3("spotLight.spot_dir",camera_front);
        s.set_float("spotLight.cos_inner_cut_off",glm::cos(glm::radians(12.5f)));
        s.set_float("spotLight.cos_outer_cut_off",glm::cos(glm::radians(17.5f)));
        s.set_vec3("spotLight.position",eye_pos);

        light_source_shader.use();
        light_source_shader.set_mat4("view",view_matrix);

        border_shader.use();
        border_shader.set_mat4("view",view_matrix);


        //*************************


        glStencilOp(GL_KEEP, GL_KEEP,GL_REPLACE);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
        for(const auto & position :cubePositions){
            auto tmp_model_map = glm::translate(glm::mat4(1.f),position);
            s.use();
            s.set_mat4("model",tmp_model_map);
            glStencilFunc(GL_ALWAYS,1,0xFF);
            glStencilMask(0xFF);
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES,0,36);

            glDisable(GL_DEPTH_TEST);
            glStencilFunc(GL_NOTEQUAL,1,0xFF);
            glStencilMask(0x00);
            glBindVertexArray(border_VAO);
            border_shader.use();
            float scale = 1.02f;
            border_shader.set_mat4("model",glm::scale(tmp_model_map,{scale,scale,scale}));
            glDrawArrays(GL_TRIANGLES,0,36);
            glStencilMask(0xFF);

            glEnable(GL_DEPTH_TEST);
            //glStencilFunc(GL_ALWAYS,1,0xFF);
        }

//        light_source_shader.use();
//        glBindVertexArray(light_VAO);
//        for(int i = 0 ; i < 4; ++i){
//            light_source_shader.set_mat4("model",glm::scale(glm::translate(glm::mat4(1.f),pointLightPositions[i]),glm::vec3(0.3f)));
//            glDrawArrays(GL_TRIANGLES,0,36);
//            //s.set_vec3(("pointLights["s+ to_string(i)+"].position").c_str(),pointLightPositions[i]);
//        }

        glfwSwapBuffers(window);
        glfwPollEvents();
        glClearColor(0.0f,0.0f,0.0f,0.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
    }
    
}