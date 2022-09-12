#include "Shader.h"
#include <exception>
#include <fstream>
#include <iostream>
#include <iterator>
#include <ostream>
#include <sstream>
#include <glad/glad.h>
#include<GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include "util.h"
#include <string>
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
}

void Shader::use(){
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
