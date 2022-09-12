#include <string>
#include <glm/glm.hpp>
class Shader{
    public:
    Shader(const char* vertex_shader_path,const char* fragment_shader_path);
    void use();
    unsigned int program_id;
    bool set_vec3(const char* vari_name,const glm::vec3 &v);
    bool set_mat4(const char* vari_name,const glm::mat4 &mat);
    bool set_float(const char* vari_name,const float &f);
};