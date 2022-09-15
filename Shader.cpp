#include "Shader.h"
#include <exception>
#include <fstream>
#include <iostream>
#include <iterator>
#include <ostream>
#include <glad/glad.h>
#include<GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include "util.h"
#include <string>
//#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
using namespace std;
void checkCompileErrors(unsigned int shader, std::string type)
    {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
Shader::Shader(const char* vertex_shader_path,const char* fragment_shader_path){
    std::ifstream vertext_source_stream(vertex_shader_path);
    std::ifstream fragment_source_stream(fragment_shader_path);
    std::string vertex_source((std::istreambuf_iterator<char>(vertext_source_stream)),std::istreambuf_iterator<char>());
    auto p_vertex_source = vertex_source.c_str();
    std::string fragment_source((std::istreambuf_iterator<char>(fragment_source_stream)),std::istreambuf_iterator<char>());
    auto p_fragment_source = fragment_source.c_str();
    //std::cout<<"frag:"<<p_fragment_source<<std::endl;
    //std::cout<<"vertex"<<p_vertex_source<<std::endl;
    unsigned int vertex_shader,fragment_shader;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);;
    
    glShaderSource(vertex_shader,1,&p_vertex_source,NULL);
    glShaderSource(fragment_shader,1,&p_fragment_source,NULL);
    
    glCompileShader(vertex_shader);
    checkCompileErrors(vertex_shader, "VERTEX");
    glCompileShader(fragment_shader);
    checkCompileErrors(fragment_shader, "FRAGMENT");
    
    program_id = glCreateProgram();
    glAttachShader(program_id,vertex_shader);
    glAttachShader(program_id,fragment_shader);
    glLinkProgram(program_id);
    glDeleteShader(fragment_shader);
    glDeleteShader(vertex_shader);

    this->use();
    shader_macro.assign({GL_TEXTURE0,GL_TEXTURE1,GL_TEXTURE2,GL_TEXTURE3,GL_TEXTURE4,GL_TEXTURE5,GL_TEXTURE6,GL_TEXTURE7,GL_TEXTURE8,GL_TEXTURE9,GL_TEXTURE10,
                         GL_TEXTURE11,GL_TEXTURE12,GL_TEXTURE13,GL_TEXTURE14,GL_TEXTURE15});
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Shader::use() const{
    glUseProgram(this->program_id);
}

bool Shader::set_vec3(const char* vari_name,const glm::vec3 &v){
    try {
        int location = glGetUniformLocation(this->program_id,vari_name);
         glUniform3fv(location,1,glm::value_ptr(v));
    } catch (std::exception e) {
        std::cout << e.what() << std::endl;
        return false;
    }
    return true;
}
bool Shader::set_mat4(const char* vari_name,const glm::mat4 &mat){
    try {
        glUniformMatrix4fv(glGetUniformLocation(this->program_id,vari_name),1,GL_FALSE,glm::value_ptr(mat));
    } catch (std::exception e) {
        std::cout << e.what() << std::endl;
        return false;
    }
    return true;
}

bool Shader::set_float(const char *vari_name, const float& f) {
    try {
        glUniform1f(glGetUniformLocation(this->program_id,vari_name),f);
    } catch (std::exception e) {
        std::cout << e.what() << std::endl;
        return false;
    }
    return true;
}

bool Shader::set_int(const char *vari_name, const int &i) const{
    try {
        glUniform1i(glGetUniformLocation(this->program_id,vari_name),i);
    } catch (std::exception e) {
        std::cout << e.what() << std::endl;
        return false;
    }
    return true;
}

unsigned int Shader::set_texture(const char *path, const char *sampler_name,unsigned int gl_texture_type) {
    this->use();

    int width,height,channels;
    auto texture_data = stbi_load(path,&width,&height,&channels,0);
    if(texture_data == nullptr) return -1;
    unsigned int texture_id = 0;
    glActiveTexture(shader_macro[cur_texture_index]);
    glGenTextures(1,&texture_id);
    glBindTexture(gl_texture_type,texture_id);
    int image_format = 0;
    if(channels == 3) image_format = GL_RGB;
    else if(channels == 4) image_format = GL_RGBA;
    else if(channels == 1) image_format = GL_ALPHA;
    glTexImage2D(GL_TEXTURE_2D,0,image_format,width,height,0,image_format,GL_UNSIGNED_BYTE,texture_data);
    glGenerateMipmap(GL_TEXTURE_2D);
    return texture_id;
}

bool Shader::set_arrays_vec3(int array_len,const char* array_name,const char* attrib_name,const glm::vec3& v){
    string tmp_s(array_name);
    for(int i = 0; i < array_len; ++i){
        set_vec3((tmp_s+'['+ to_string(i)+"]."+attrib_name).c_str(),v);
    }
    return true;
}
bool Shader::set_arrays_float(int array_len,const char* array_name,const char* attrib_name,const float f){
    string tmp_s(array_name);
    for(int i = 0; i < array_len; ++i){
        set_float((tmp_s+'['+ to_string(i)+"]."+attrib_name).c_str(),f);
    }
    return true;
}

