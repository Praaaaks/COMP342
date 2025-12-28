#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<vector>

float sx, sy;

struct Point
{
    float x;
    float y;
};

using Polygon = std::vector<Point>;

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

void drawPolygon(const Polygon& poly){
    glBegin(GL_LINE_LOOP);
    for(const auto& p : poly){
        float x = sx * p.x;
        float y = sy * p.y;
        glVertex2f(x, y);
    }
    glEnd();
}

bool inside(Point p, int edge){
    switch(edge){
        case 0: return p.x >= winMin.x; //Left
        case 1: return p.x <= winMax.x; //Right
        case 2: return p.y >= winMin.y; //Bottom
        case 3: return p.y <= winMax.y; //Top
        default: return false;
    }
}

void intersection(Point s, Point e, int edge, Point &i){
    float dx = e.x - s.x;
    float dy = e.y - s.y;

    switch(edge){
        case 0: // Left
            i.x = winMin.x;
            i.y = s.y + dy * (winMin.x - s.x) / dx;
            break;
        case 1: // Right
            i.x = winMax.x;
            i.y = s.y + dy * (winMax.x - s.x) / dx;
            break;
        case 2: // Bottom
            i.y = winMin.y;
            i.x = s.x + dx * (winMin.y - s.y) / dy;
            break;
        case 3: // Top
            i.y = winMax.y;
            i.x = s.x + dx * (winMax.y - s.y) / dy;
            break;
    }
}

Polygon clipEdge(const Polygon& poly, int edge){
    Polygon output;
    int n = poly.size();
    for(int i = 0; i < n; ++i){
        Point curr = poly[i];
        Point prev = poly[(i + n - 1) % n];

        if(inside(curr, edge)){
            if(inside(prev, edge)){
                output.push_back(curr);
            } else {
                Point inter;
                intersection(prev, curr, edge, inter);
                output.push_back(inter);
                output.push_back(curr);
            }
        } else {
            if(inside(prev, edge)){
                Point inter;
                intersection(prev, curr, edge, inter);
                output.push_back(inter);
            }
        }
    }
    return output;
}

Polygon clipPolygon(const Polygon& poly){
    Polygon output = poly;
    for(int edge = 0; edge < 4; ++edge){
        output = clipEdge(output, edge);
        if(output.empty()) break;
    }
    return output;
}

int main(){
    if(!glfwInit()) return -1;
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "Sutherland Hodgeman", nullptr, nullptr);
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

        glColor3f(1.0f, 1.0f, 1.0f);
        drawClippingWindow();

        Polygon poly = {
            {-100.0f, 50.0f},
            {50.0f, 150.0f},
            {200.0f, 0.0f},
            {0.0f, -150.0f}, 
            {-150.0f, -50.0f}
        };
        drawPolygon(poly);

        glColor3f(1.0f, 0.0f, 0.0f);
        Polygon clipped = clipPolygon(poly);
        drawPolygon(clipped);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}