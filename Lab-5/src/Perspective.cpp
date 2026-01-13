#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<windows.h>
#include<GL/glu.h>
#include<iostream>

struct Point3D
{
    float x;
    float y;
    float z;
};

float posX = 150.0f;
float posY = 150.0f;
float posZ = 200.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (double)width / height, 1.0, 1000.0);
}

void drawPoint(Point3D &p){
    glBegin(GL_POINTS);
    glVertex3f(p.x, p.y, p.z);
    glEnd();
}

Point3D perspectiveProject(Point3D &p, float prp, float vp){
    Point3D point;
    float matrix[4][4] = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    };

    matrix[2][2] = -vp / (prp - vp);
    matrix[2][3] = (prp * vp) / (prp - vp);
    matrix[3][2] = -1 / (prp - vp);
    matrix[3][3] = prp / (prp - vp);

    float x = p.x * matrix[0][0] + p.y * matrix[0][1] + p.z * matrix[0][2] + 1 * matrix[0][3];
    float y = p.x * matrix[1][0] + p.y * matrix[1][1] + p.z * matrix[1][2] + 1 * matrix[1][3];
    float z = p.x * matrix[2][0] + p.y * matrix[2][1] + p.z * matrix[2][2] + 1 * matrix[2][3];
    float h = p.x * matrix[3][0] + p.y * matrix[3][1] + p.z * matrix[3][2] + 1 * matrix[3][3];

    point.x = x / h;
    point.y = y / h;
    point.z = z / h;

    return point;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action != GLFW_PRESS) {
        return;
    }

    if (action == GLFW_PRESS) {
        switch(key) {
            case GLFW_KEY_X:
                posX = 300.0f;
                posY = 0.0f;
                posZ = 0.0f;
                break;
            case GLFW_KEY_Z:
                posX = 0.0f;
                posY = 0.0f;
                posZ = 300.0f;
                break;
            case GLFW_KEY_1:
                posX = 150.0f;
                posY = 150.0f;
                posZ = 200.0f;
                break;
            case GLFW_KEY_2:
                posX = -150.0f;
                posY = 100.0f;
                posZ = 200.0f;
                break;
            case GLFW_KEY_3:
                posX = 0.0f;
                posY = -75.0f;
                posZ = 250.0f;
                break;
            default:
                break;
        }
    }
}

void drawAxes(float length)
{
    glBegin(GL_LINES);

    // X axis (Red)
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-length, 0.0f, 0.0f);
    glVertex3f(length, 0.0f, 0.0f);

    // Y axis (Green)
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, -length, 0.0f);
    glVertex3f(0.0f, length, 0.0f);

    // Z axis (Blue)
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, -length);
    glVertex3f(0.0f, 0.0f, length);

    glEnd();
}

void drawViewPlane(float size, float vp){
    float s = size / 2.0f;

    glColor3f(0.7f, 1.0f, 0.2f);
    glBegin(GL_LINE_LOOP);
    glVertex3f(-s, -s, vp);
    glVertex3f(s, -s, vp);
    glVertex3f(s, s, vp);
    glVertex3f(-s, s, vp);
    glEnd();
}

void drawProjectionRay(Point3D &p, float prp){
    glColor3f(0.7f, 0.7f, 0.7f);
    glBegin(GL_LINES);
    glVertex3f(0, 0, prp); // PRP
    glVertex3f(p.x, p.y, p.z);
    glEnd();
}

int main(){
    if(!glfwInit()) return -1;
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "Perspective Projection", nullptr, nullptr);
    if (!window) { glfwTerminate(); return -1; }
    
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    glfwSetKeyCallback(window, key_callback);
    
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1, 0.1, 0.1, 1);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    framebuffer_size_callback(window, width, height);

    glPointSize(5.0f);
    
    while(!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(
            posX, posY, posZ,
            0.0f, 0.0f, 0.0f, 
            0.0f, 1.0f, 0.0f
        );

        Point3D P1 = {30, 40, -100};
        Point3D P2 = {-50, -80, -150};
        float prp = 100;
        float vp  = 0;

        Point3D projP1 = perspectiveProject(P1, prp, vp);
        Point3D projP2 = perspectiveProject(P2, prp, vp);

        drawAxes(150);
        drawViewPlane(120, vp);

        glColor3f(1.0f, 1.0f, 0.0f);
        drawPoint(P1);
        drawPoint(P2);

        Point3D prpPoint = {0, 0, prp};
        glColor3f(1.0f, 0.0f, 1.0f);
        drawPoint(prpPoint);

        drawProjectionRay(P1, prp);
        drawProjectionRay(P2, prp);

        glColor3f(0.0f, 1.0f, 1.0f);
        drawPoint(projP1);
        drawPoint(projP2);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}