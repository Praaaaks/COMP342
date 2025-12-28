#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>

float sx, sy;

struct Point
{
    float x;
    float y;
};

enum Clipper{
    LEFT = 1, RIGHT = 2, BOTTOM = 4, TOP = 8
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

int getRegionCode(Point p){
    int code = 0;
    
    if(p.x < winMin.x)
        code |= 1;
    else if(p.x > winMax.x)
        code |= 2;
    
    if(p.y < winMin.y)
        code |= 4;
    else if(p.y > winMax.y)
        code |= 8;
    
    return code;
}

Clipper getClipper(int regCode){
    if(regCode & 1)
        return LEFT;
    else if(regCode & 2)
        return RIGHT;
    else if(regCode & 4)
        return BOTTOM;
    else
        return TOP;
}

void LineClip(Point p1, Point p2){
    int reg1 = getRegionCode(p1);
    int reg2 = getRegionCode(p2);

    float slope = (p2.y - p1.y) / (p2.x - p1.x);

    while(true){
        if((reg1 | reg2) == 0){
            drawLine(p1, p2);
            return;
        }

        if((reg1 & reg2) != 0){
            return;
        }

        int outCode = reg1 ? reg1 : reg2;
        Point *p = reg1 ? &p1 : &p2;

        if (outCode & LEFT) {
        p->y += slope * (winMin.x - p->x);
        p->x = winMin.x;
        }
        else if (outCode & RIGHT) {
            p->y += slope * (winMax.x - p->x);
            p->x = winMax.x;
        }
        else if (outCode & BOTTOM) {
            p->x += (winMin.y - p->y) / slope;
            p->y = winMin.y;
        }
        else if (outCode & TOP) {
            p->x += (winMax.y - p->y) / slope;
            p->y = winMax.y;
        }

        reg1 = getRegionCode(p1);
        reg2 = getRegionCode(p2);
    }
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
        LineClip(p1, p2);
        LineClip(p3, p4);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}