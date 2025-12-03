#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    GLFWmonitor* primary = glfwGetPrimaryMonitor();
    if(!primary) return -1;

    const GLFWvidmode* mode = glfwGetVideoMode(primary);
    if(!mode) return -1;

    std::cout << "Screen Resolution: " << mode->width << " x " << mode->height << std::endl;
    std::cout << "Refresh Rate: " << mode->refreshRate <<std::endl;

    glfwTerminate();
    return 0;
}
