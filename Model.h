//
// Created by 17863 on 2022/9/14.
//

#ifndef LEARNOPEN_MODEL_H
#define LEARNOPEN_MODEL_H
#include <vector>
#include <string>
#include <unordered_map>
#include "Mesh.h"

class aiNode;
class Shader;
struct aiScene;
class aiMesh;
class aiMaterial;
struct Texture;
enum aiTextureType;
class Model {
private:
    std::vector<Mesh> meshes;
    std::string directory;
    void load_model(const char* name);
    void process_node(aiNode* node,const aiScene* scene);
    Mesh process_mesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat,aiTextureType type,std::string type_name);
    std::unordered_map<std::string ,unsigned int> loaded_texture_map;
public:
    Model(const char* path);
    void Draw(Shader& shader);
};


#endif //LEARNOPEN_MODEL_H
