#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<cmath>

struct Point
{
    float x;
    float y;
};

enum TransformMode {
    NONE,
    TRANSLATE,
    SCALE,
    ROTATE,
    SHEAR,
    REFLECT_X,
    REFLECT_Y
};

TransformMode currentMode = NONE;

float sx, sy;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    sx = 2.0f / width;
    sy = 2.0f / height;
}

void drawAxes(){
    glBegin(GL_LINES);
    // X axis
    glVertex2f(-1.0f, 0.0f);
    glVertex2f(1.0f, 0.0f);
    // Y axis
    glVertex2f(0.0f, -1.0f);
    glVertex2f(0.0f, 1.0f);
    glEnd();
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action != GLFW_PRESS)
        return;

    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_1:
                currentMode = TRANSLATE;
                break;
            case GLFW_KEY_2:
                currentMode = SCALE;
                break;
            case GLFW_KEY_3:
                currentMode = ROTATE;
                break;
            case GLFW_KEY_4:
                currentMode = SHEAR;
                break;
            case GLFW_KEY_5:
                currentMode = REFLECT_X;
                break;
            case GLFW_KEY_6:
                currentMode = REFLECT_Y;
                break;
            case GLFW_KEY_0:
                currentMode = NONE;
                break;
            default:
                break;
        }
    }
}

void drawTriangle(Point p1, Point p2, Point p3){
    glBegin(GL_TRIANGLES);
    glVertex2f(sx * p1.x, sy * p1.y);
    glVertex2f(sx * p2.x, sy * p2.y);
    glVertex2f(sx * p3.x, sy * p3.y);
    glEnd();
}

Point transformPoint(Point p, float M[3][3]){
    Point result;
    result.x = M[0][0] * p.x + M[0][1] * p.y + M[0][2];
    result.y = M[1][0] * p.x + M[1][1] * p.y + M[1][2];
    return result;
}

void translate(Point p1, Point p2, Point p3, float tx, float ty){
    float T[3][3] = {
        {1, 0, tx},
        {0, 1, ty},
        {0, 0, 1}
    };

    Point tp1 = transformPoint(p1, T);
    Point tp2 = transformPoint(p2, T);
    Point tp3 = transformPoint(p3, T);

    drawTriangle(tp1, tp2, tp3);
}

void scale(Point p1, Point p2, Point p3, float sx, float sy){
    float T[3][3] = {
        {sx, 0, 0},
        {0, sy, 0},
        {0, 0, 1}
    };

    Point tp1 = transformPoint(p1, T);
    Point tp2 = transformPoint(p2, T);
    Point tp3 = transformPoint(p3, T);

    drawTriangle(tp1, tp2, tp3);
}

void rotate(Point p1, Point p2, Point p3, float theta){
    float T[3][3] = {
        {cos(theta), -sin(theta), 0},
        {sin(theta), cos(theta), 0},
        {0, 0, 1}
    };

    Point tp1 = transformPoint(p1, T);
    Point tp2 = transformPoint(p2, T);
    Point tp3 = transformPoint(p3, T);

    drawTriangle(tp1, tp2, tp3);
}

void shear(Point p1, Point p2, Point p3, float shx, float shy){
    float T[3][3] = {
        {1, shx, 0},
        {shy, 1, 0},
        {0, 0, 1}
    };

    Point tp1 = transformPoint(p1, T);
    Point tp2 = transformPoint(p2, T);
    Point tp3 = transformPoint(p3, T);

    drawTriangle(tp1, tp2, tp3);
}

void reflectX(Point p1, Point p2, Point p3){
    float T[3][3] = {
        {1, 0, 0},
        {0, -1, 0},
        {0, 0, 1}
    };

    Point tp1 = transformPoint(p1, T);
    Point tp2 = transformPoint(p2, T);
    Point tp3 = transformPoint(p3, T);

    drawTriangle(tp1, tp2, tp3);
}

void reflectY(Point p1, Point p2, Point p3){
    float T[3][3] = {
        {-1, 0, 0},
        {0, 1, 0},
        {0, 0, 1}
    };

    Point tp1 = transformPoint(p1, T);
    Point tp2 = transformPoint(p2, T);
    Point tp3 = transformPoint(p3, T);

    drawTriangle(tp1, tp2, tp3);
}

int main(){
    if(!glfwInit()) return -1;
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "2D Transform", nullptr, nullptr);
    if (!window) { glfwTerminate(); return -1; }
    
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    glfwSetKeyCallback(window, key_callback);
    
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    framebuffer_size_callback(window, width, height);

    glClearColor(0.1, 0.1, 0.1, 1);
    
    while(!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT);

        glColor3f(1.0f, 1.0f, 1.0f);
        drawAxes();

        Point p1 = {25.0f, 25.0f};
        Point p2 = {25.0f, 100.0f};
        Point p3 = {100.0f, 25.0f};

        glColor3f(0.5f, 0.8f, 0.1f);
        drawTriangle(p1, p2, p3);

        glColor3f(0.1f, 0.8f, 0.5f);
        
        switch(currentMode) {
            case TRANSLATE:
                translate(p1, p2, p3, 150.0f, 50.0f);
                break;
            case SCALE:
                scale(p1, p2, p3, 1.5f, 1.5f);
                break;
            case ROTATE:
                rotate(p1, p2, p3, M_PI / 4);
                break;
            case SHEAR:
                shear(p1, p2, p3, 0.5f, 0.0f);
                break;
            case REFLECT_X:
                reflectX(p1, p2, p3);
                break;
            case REFLECT_Y:
                reflectY(p1, p2, p3);
                break;
            default:
                break;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}