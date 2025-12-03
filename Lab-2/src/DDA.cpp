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

void lineDDA(int x1, int y1, int x2, int y2){
    int dx = x2 - x1;
    int dy = y2 - y1;

    int stepsize = fabs(dy) > fabs(dx) ? fabs(dy) : fabs(dx);
    float xInc = (float)dx / stepsize;
    float yInc = (float)dy / stepsize;

    float x = (float)x1;
    float y = (float)y1;

    for(int i = 0; i <= stepsize; i++){
        drawPoint((float)round(x), (float)round(y));
        x += xInc;
        y += yInc;
    }
}

int main(){
    if(!glfwInit()) return -1;
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "DDA", nullptr, nullptr);
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

    glClearColor(0, 0, 0, 1);
    
    while(!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT);
        
        glColor3f(1.0f, 0.5f, 0.5f);
        lineDDA(0, 0, 100, 120);

        glColor3f(0.3f, 0.7f, 0.2f);
        lineDDA(-180, 50, 180, -50);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}