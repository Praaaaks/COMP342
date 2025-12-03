#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<cmath>

float sx, sy;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    sx = 2.0f / width;
    sy = 2.0f / height;
}

void drawPoint(float x, float y){
    x = sx * x ;
    y = sy * y ;
    glBegin(GL_POINTS);
    glVertex2f(x, y);
    glEnd();
}

void draw8points(int x, int xc, int y, int yc){
    drawPoint(x + xc, y + yc);
    drawPoint(-x + xc, y + yc);
    drawPoint(x + xc, -y + yc);
    drawPoint(-x + xc, -y + yc);
    drawPoint(y + xc, x + yc);
    drawPoint(-y + xc, x + yc);
    drawPoint(y + xc, -x + yc);
    drawPoint(-y + xc, -x + yc);
}

void midpointCircle(int xc, int yc, int r){
    int x = 0, y = r;
    int p = 1 - r;

    while (x < y)
    {
        draw8points(x, xc, y, yc);
        if(p < 0){
            x++;
            p += 2 * x + 1;
        } else {
            x++;
            y--;
            p += 2 * x - 2 * y + 1;
        }
    }
}

int main(){
    if(!glfwInit()) return -1;
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "Circle", nullptr, nullptr);
    if (!window) { glfwTerminate(); return -1; }
    
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    framebuffer_size_callback(window, width, height);

    glClearColor(0.1, 0.1, 0.1, 1);
    
    while(!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT);

        glColor3f(0.2f, 0.25f, 0.8f);
        midpointCircle(100, -100, 100);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}