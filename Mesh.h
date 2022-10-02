//
// Created by 17863 on 2022/9/14.
//

#ifndef LEARNOPEN_MESH_H
#define LEARNOPEN_MESH_H
#include<vector>
#include <glm/glm.hpp>
#include <string>
class Shader;
struct Vertex{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex_coord;
};
struct Texture{
    unsigned int id;
    std::string type;
    const char* path;
};
class Mesh {
private:
    std::vector<Vertex> vertices;
    std::vector<int> indices;
    std::vector<Texture> textures;
    unsigned int VBO,VAO,EBO;
    void setup_mesh();
public:
    Mesh(std::vector<Vertex> vertices, std::vector<int> indices, std::vector<Texture> textures);
    void draw(const Shader& shader);
    void draw(const Shader& shader,int instance_num);
    const unsigned int get_VAO() const;
};


#endif //LEARNOPEN_MESH_H
