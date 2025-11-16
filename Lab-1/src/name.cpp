#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void drawRect(float x1, float y1, float x2, float y2) {
    glBegin(GL_POLYGON);
    glVertex2f(x1, y1);
    glVertex2f(x1, y2);
    glVertex2f(x2, y2);
    glVertex2f(x2, y1);
    glEnd();
}

void drawA(float x1, float x2){
    glBegin(GL_POLYGON);    //Left Leg
    glVertex2f(x1, -0.4f);
    glVertex2f(x1 + 0.1f, 0.4f);
    glVertex2f(x1 + 0.1f, 0.2f);
    glVertex2f(x1 + 0.04f, -0.4);
    glEnd();
    glBegin(GL_POLYGON);    //Right Leg
    glVertex2f(x2 - 0.1f, 0.4f);
    glVertex2f(x2, -0.4f);
    glVertex2f(x2 - 0.04f, -0.4f);
    glVertex2f(x2 - 0.1f, 0.2);
    glEnd();
    drawRect(x1 + 0.04f, -0.2f, x2 - 0.04f, -0.16f);    //Middle Bar
}

int main() {
    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Name", nullptr, nullptr);
    if (!window) { glfwTerminate(); return -1; }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    glClearColor(0, 0, 0, 1);

    while(!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT);
        //draw P
        glColor3f(1.0f, 1.0f, 0.0f);
        drawRect(-0.96f, -0.4f, -0.92f, 0.4f);  //Stem
        drawRect(-0.92f, 0.4f, -0.76f, 0.325f); //Top
        drawRect(-0.8f, 0.325f, -0.76f, 0.075f);//Side
        drawRect(-0.92f, 0.075f, -0.76f, 0.0f); //Bot

        //draw R
        glColor3f(1.0f, 0.0f, 1.0f);
        drawRect(-0.72f, -0.4f, -0.68f, 0.4f);  //Stem
        drawRect(-0.68f, 0.4f, -0.52f, 0.325f); //Top
        drawRect(-0.56f, 0.325f, -0.52f, 0.075f);//Side
        drawRect(-0.68f, 0.075f, -0.52f, 0.0f); //Bot
        glBegin(GL_POLYGON);
        glVertex2f(-0.7f, 0.0f);
        glVertex2f(-0.66f, 0.0f);
        glVertex2f(-0.52f, -0.4f);
        glVertex2f(-0.56f, -0.4f);
        glEnd();

        //draw A
        glColor3f(0.0f, 1.0f, 1.0f);
        drawA(-0.48f, -0.28f);

        //draw K
        glColor3f(1.0f, 0.0f, 0.0f);
        drawRect(-0.24f, -0.4f, -0.2f, 0.4f);   //Stem
        glBegin(GL_POLYGON);    //Top
        glVertex2f(-0.2f, 0.05f);
        glVertex2f(-0.08f, 0.4f);
        glVertex2f(-0.04f, 0.4f);
        glVertex2f(-0.2f, -0.05f);
        glEnd();
        glBegin(GL_POLYGON);    //Bottom
        glVertex2f(-0.2f, 0.05f);
        glVertex2f(-0.04f, -0.4f);
        glVertex2f(-0.08f, -0.4f);
        glVertex2f(-0.2f, -0.05f);
        glEnd();

        //draw A
        glColor3f(0.0f, 1.0f, 0.0f);
        drawA(0.0f, 0.20f);

        //draw L
        glColor3f(0.0f, 0.0f, 1.0f);
        drawRect(0.24f, -0.4f, 0.28f, 0.4f);    //Stem
        drawRect(0.28f, -0.4f, 0.44f, -0.325f);   //Base

        //draw P
        glColor3f(1.0f, 0.3f, 0.3f);
        drawRect(0.48f, -0.4f, 0.52f, 0.4f);  //Stem
        drawRect(0.52f, 0.4f, 0.68f, 0.325f); //Top
        drawRect(0.64f, 0.325f, 0.68f, 0.075f);//Side
        drawRect(0.52f, 0.075f, 0.68f, 0.0f); //Bottom

        //draw A
        glColor3f(0.5f, 0.6f, 0.4f);
        drawA(0.72f, 0.92f);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}