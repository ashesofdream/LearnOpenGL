#include <string>
#include <glm/glm.hpp>
#include <vector>
class Shader{
    public:
    Shader(const char* vertex_shader_path,const char* fragment_shader_path,const char* geometry_shader_path = nullptr);
    void use() const;
    unsigned int program_id;
    bool set_vec3(const char* vari_name,const glm::vec3 &v);
    bool set_mat4(const char* vari_name,const glm::mat4 &mat);
    bool set_float(const char* vari_name,const float &f);
    bool set_int(const char* vari_name,const int &i) const;
    bool set_arrays_vec3(int array_len, const char *array_name, const char *attrib_name, const glm::vec3 &v);
    bool set_arrays_float(int array_len, const char *array_name, const char *attrib_name, const float f);
    unsigned int set_texture(const char *path, const char *sampler_name,unsigned int gl_texture_type=0x0DE1);//default texture type is GL_TEXTURE_2D
private:
    std::vector<unsigned int> shader_macro;
    int cur_texture_index = 0;

};