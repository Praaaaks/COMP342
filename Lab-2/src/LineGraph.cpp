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
    x = sx * x - 1;
    y = sy * y - 1;
    glBegin(GL_POINTS);
    glVertex2f(x, y);
    glEnd();
}

void lowSlopeBLA(int x, int y, int dx, int dy, int sgx, int sgy){
    int p = 2 * dy - dx;
    for(int i = 0; i < dx; i++){
        if(p < 0){
            x += sgx;
            drawPoint(x, y);
            p += 2 * dy;
        } else {
            x += sgx;
            y += sgy;
            drawPoint(x, y);
            p += 2 * (dy - dx);
        }
    }
}

void highSlopeBLA(int x, int y, int dx, int dy, int sgx, int sgy){
    int p = 2 * dx - dy;
    for(int i = 0; i < dy; i++){
        if(p < 0){
            y += sgy;
            drawPoint(x, y);
            p += 2 * dx;
        } else {
            x += sgx;
            y += sgy;
            drawPoint(x, y);
            p += 2 * (dx - dy);
        }
    }
}

void lineBLA(int x1, int y1, int x2, int y2){
    int dy = y2 - y1;
    int dx = x2 - x1;

    int sgx = (dx > 0)? 1 : -1;
    int sgy = (dy > 0)? 1 : -1;

    dy = abs(dy);
    dx = abs(dx);

    drawPoint(x1, y1);

    if (dy < dx)
        lowSlopeBLA(x1, y1, dx, dy, sgx, sgy);
    else
        highSlopeBLA(x1, y1, dx, dy, sgx, sgy);
}

void drawLineGraph(int data[], int size, int startX, int stepX){
    for(int i = 0; i < size - 1; i++){
        lineBLA(startX, data[i], startX + stepX, data[i + 1]);
        startX += stepX;
    }
}

int main(){
    if(!glfwInit()) return -1;
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "Line Graph", nullptr, nullptr);
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
        
        int data[] = {250, 100, 400, 300, 250};
        int size = sizeof(data)/sizeof(data[0]);
        
        glColor3f(1.0f, 1.0f, 1.0f);
        drawLineGraph(data, size, 50, 100);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}