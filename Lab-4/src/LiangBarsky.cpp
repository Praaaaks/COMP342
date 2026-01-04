#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<algorithm>

float sx, sy;
bool clipEnabled = false;
int currentLine = 1;

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

void LineClip(Point p1, Point p2){
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;

    float p[4] = {-dx, dx, -dy, dy};
    float q[4] = {
        p1.x - winMin.x,
        winMax.x - p1.x,
        p1.y - winMin.y,
        winMax.y - p1.y
    };

    float u1 = 0.0f;
    float u2 = 1.0f;

    for (int i = 0; i < 4; i++) {
        if (p[i] == 0) {
            if (q[i] < 0)
                return; // Parallel and outside
        } else {
            float u = q[i] / p[i];
            if (p[i] < 0)
                u1 = std::max(u1, u);
            else
                u2 = std::min(u2, u);
        }
    }

    if (u1 > u2)
        return;

    Point c1 = {
        p1.x + u1 * dx,
        p1.y + u1 * dy
    };

    Point c2 = {
        p1.x + u2 * dx,
        p1.y + u2 * dy
    };

    drawLine(c1, c2);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (action == GLFW_PRESS)
    {
        if (key == GLFW_KEY_C)
            clipEnabled = true;

        if (key == GLFW_KEY_R)
            clipEnabled = false;

        if (key == GLFW_KEY_1)
            currentLine = 1;

        if (key == GLFW_KEY_2)
            currentLine = 2;
    }
}

int main(){
    if(!glfwInit()) return -1;
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "Liang Barksy", nullptr, nullptr);
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

    Point p1 = {40.0f, 0.0f};
    Point p2 = {-265.0f, -10.0f};
    Point p3 = {-100.0f, -200.0f};
    Point p4 = {300.0f, 0.0f};
    
    while(!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT);

        glColor3f(1.0f, 1.0f, 1.0f);
        drawClippingWindow();

        Point a, b;

        switch(currentLine){
            case 1:
                a = p1;
                b = p2;
                break;
            case 2:
                a = p3;
                b = p4;
                break;
            default:
                a = p1;
                b = p2;
                break;
        }
        
        
        if(!clipEnabled){
            drawLine(a, b);
        } else {
            glColor3f(1.0f, 0.0f, 0.0f);
            LineClip(a, b);
        }
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}