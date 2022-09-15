//
// Created by 17863 on 2022/9/14.
//

#include "Model.h"
#include "Mesh.h"
#include "util.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace std;
Model::Model(const char *path) {
    load_model(path);
}

void Model::load_model(const char *name) {
    Assimp::Importer importer;
    const auto* scene = importer.ReadFile(name,aiProcess_Triangulate|aiProcess_FlipUVs);
    if(!scene||scene->mFlags&AI_SCENE_FLAGS_INCOMPLETE||!scene->mRootNode){
        util::debug("Model::load_model __ load fail ,may message not complete");
        return;
    }
    process_node(scene->mRootNode,scene);

}

void Model::process_node(aiNode *node, const aiScene *scene) {
    if(node->mNumMeshes){
        for(unsigned int i = 0 ;  i < node->mNumMeshes;++i){
            auto p_ai_mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.emplace_back(process_mesh(p_ai_mesh,scene));
        }
    }
    for(unsigned int i = 0 ; i < node->mNumChildren ; ++i) process_node(node->mChildren[i],scene);
}

Mesh Model::process_mesh(aiMesh *mesh, const aiScene *scene) {
    vector<Vertex> vertices;
    vector<int> indices;
    vector<Texture> text;
    for(unsigned int i = 0 ; i < mesh->mNumVertices;++i ){
        auto cur_ai_vertex = mesh->mVertices[i];
        glm::vec3 pos{cur_ai_vertex.x,cur_ai_vertex.y,cur_ai_vertex.z};
        glm::vec3 normal;
        if(mesh->HasNormals()) {
            auto cur_ai_normal = mesh->mNormals[i];
            normal = {cur_ai_normal.x,cur_ai_normal.y,cur_ai_normal.z};
        }
        auto cur_ai_tex_coord = mesh->mTextureCoords[0];
        glm::vec2 tex_coord{cur_ai_tex_coord?cur_ai_tex_coord[i].x:0,cur_ai_tex_coord?cur_ai_tex_coord[i].y:0};
        vertices.emplace_back(Vertex{pos,normal,tex_coord});
    }

    for(unsigned int i = 0 ; i < mesh->mNumFaces; i++) {
        auto face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    };
    if(mesh->mMaterialIndex >= 0){
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        vector<Texture> diffuseMap = loadMaterialTextures(material,aiTextureType_DIFFUSE,"texture_diffuse");
        vector<Texture> specularMap = loadMaterialTextures(material,aiTextureType_DIFFUSE,"texture_specular");
        text.insert(text.end(), diffuseMap.begin(), diffuseMap.end());
        text.insert(text.end(),specularMap.begin(),specularMap.end());
    }
    return {vertices,indices,text};


}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string type_name) {
    vector<Texture> textures;
    for(unsigned int i = 0 ; i < mat->GetTextureCount(type);++i){
        aiString str;
        mat->GetTexture(type,i,&str);
        if(auto find_ret = loaded_texture_map.find(str.C_Str());
            find_ret != loaded_texture_map.end()){
            textures.emplace_back(Texture{find_ret->second,type_name, str.C_Str()});
            continue;
        }
        unsigned int id = util::texture_from_file(str.C_Str(),"..\\resource");
        textures.emplace_back(Texture{id,type_name,str.C_Str()});
        loaded_texture_map.emplace(str.C_Str(),id);
    }
    return textures;
}

void Model::Draw(Shader &shader) {
    for( auto& mesh: meshes){
        mesh.draw(shader);
    }
}

