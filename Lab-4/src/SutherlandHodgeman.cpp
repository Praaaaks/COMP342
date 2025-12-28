#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>

float sx, sy;

struct Point
{
    float x;
    float y;
};

Point winMin = {-150.0f, -100.0f};
Point winMax = {150.0f, 100.0f};

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    sx = 2.0f / width;
    sy = 2.0f / height;
}

void drawLine(Point p1, Point p2){
    float x1 = sx * p1.x;
    float y1 = sy * p1.y;
    float x2 = sx * p2.x;
    float y2 = sy * p2.y;
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
}

void drawClippingWindow(){
    Point topleft = {winMin.x, winMax.y};
    Point bottomright = {winMax.x, winMin.y};

    drawLine(winMin, bottomright);
    drawLine(bottomright, winMax);
    drawLine(winMax, topleft);
    drawLine(topleft ,winMin);
}

int main(){
    if(!glfwInit()) return -1;
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "Cohen Sutherland", nullptr, nullptr);
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

    Point p1 = {-300.0f, 50.0f};
    Point p2 = {265.0f, 200.0f};
    Point p3 = {110.0f, -200.0f};
    Point p4 = {300.0f, 0.0f};
    
    while(!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT);

        glColor3f(1.0f, 1.0f, 1.0f);
        drawClippingWindow();

        drawLine(p1, p2);
        drawLine(p3, p4);

        glColor3f(1.0f, 0.0f, 0.0f);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}