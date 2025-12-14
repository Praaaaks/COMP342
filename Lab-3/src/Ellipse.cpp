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

void draw4points(int x, int xc, int y, int yc){
    drawPoint(x + xc, y + yc);
    drawPoint(-x + xc, y + yc);
    drawPoint(x + xc, -y + yc);
    drawPoint(-x + xc, -y + yc);
}

void midpointEllipse(int xc, int yc, int rx, int ry){
    //Region 1
    int x = 0, y = ry;
    long long rx2 = pow(rx, 2);
    long long ry2 = pow(ry, 2);
    double p1 = ry2 - rx2 * ry + 0.25f * rx2;

    while (2 * rx2 * y > 2 * ry2 * x)
    {
        draw4points(x, xc, y, yc);
        if(p1 < 0){
            x++;
            p1 += 2 * ry2 * x + ry2 + 1;
        } else {
            x++;
            y--;
            p1 += 2 * ry2 * x + ry2 + 1 - 2 * rx2 * y;
        }
    }

    //Region 2
    double p2 = ry2 * pow(x + 0.5, 2) + rx2 * pow(y - 1, 2) - rx2 * ry2;

    while (y >= 0){
        draw4points(x, xc, y, yc);
        if(p2 < 0){
            x++;
            y--;
            p2 += rx2 - 2 * rx2 * y + 2 * ry2 * x;
        } else {
            y--;
            p2 += rx2 - 2 * rx2 * y;
        }
    }
}

int main(){
    if(!glfwInit()) return -1;
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "Ellipse", nullptr, nullptr);
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

        glColor3f(0.4f, 0.1f, 0.8f);
        midpointEllipse(0, 0, 100, 150);

        glColor3f(0.8f, 0.1f, 0.4f);
        midpointEllipse(0, 0, 150, 100);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}