//
// Created by 17863 on 2022/9/14.
//

#include "Mesh.h"
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <string>
#include "Shader.h"
using namespace std;

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<int> indices, std::vector<Texture> textures){
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    setup_mesh();
};

void Mesh::setup_mesh() {
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glGenBuffers(1,&EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(Vertex)*vertices.size(),&vertices[0],GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(unsigned int)*indices.size(),&indices[0],GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),0);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex, normal));
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*) offsetof(Vertex,tex_coord));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

}
void Mesh::draw(const Shader& shader){
    shader.use();
    unsigned int diffuseNr = 1,specularNr = 1;
    for(unsigned int i = 0 ; i < textures.size(); ++i){
        glActiveTexture(GL_TEXTURE0+i);
        std::string number;
        std::string name= textures[i].type;
        if(name== "texture_diffuse"){
            number = std::to_string(diffuseNr);
        }else if(name == "texture_specular"){
            number = std::to_string(specularNr);
        }
        auto sampler_name = ("material."s+name+number);
        shader.set_int(sampler_name.c_str(),i);
        glBindTexture(GL_TEXTURE_2D,textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
