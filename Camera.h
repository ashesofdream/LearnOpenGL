#include <glm/glm.hpp>
#include <functional>
struct GLFWwindow;
class Camera{
private:
    glm::vec3 front_;
    glm::vec3 up_;
    glm::vec3 pos_;
    float fov_,aspect_,near_,far_;
    GLFWwindow* window_;
    std::function<void(Camera*)> process_func_;
public:
    Camera();
    Camera(GLFWwindow* window);
    glm::vec3& front();
    glm::vec3& up();
    glm::vec3& pos();
    GLFWwindow* window();
    glm::mat4 view_matrix();
    glm::mat4 perspective_matrix();
    void update();
};