#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <functional>
class Camera{
private:
    glm::vec3 front_;
    glm::vec3 up_;
    glm::vec3 pos_;
    GLFWwindow* window_;
    std::function<void(Camera*)> process_func_;
public:
    Camera();
    Camera(GLFWwindow* window);
    glm::vec3& front();
    glm::vec3& up();
    glm::vec3& pos();
    GLFWwindow* window();
    void update();
};