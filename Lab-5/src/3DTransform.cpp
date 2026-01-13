#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<windows.h>
#include<GL/glu.h>
#include<iostream>
#include<cmath>
#include<cctype>

struct Point3D
{
    float x;
    float y;
    float z;
};

struct Cube
{
    Point3D v[8];
};

enum TransformMode3D {
    NONE,
    TRANSLATE,
    SCALE,
    SHEAR,
    ROTATE
};

TransformMode3D currentMode3D = NONE;

static const int edges[12][2] = {
    {0,1}, {1,4}, {4,2}, {2,0},   // bottom
    {3,5}, {5,7}, {7,6}, {6,3},   // top
    {0,3}, {1,5}, {2,6}, {4,7}    // verticals
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (double)width / height, 1.0, 1000.0);

    glMatrixMode(GL_MODELVIEW);
}

void drawAxes(float length)
{
    glBegin(GL_LINES);

    // X axis (Red)
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(length, 0.0f, 0.0f);

    // Y axis (Green)
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, length, 0.0f);

    // Z axis (Blue)
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, length);

    glEnd();
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action != GLFW_PRESS) {
        return;
    }

    if (action == GLFW_PRESS) {
        switch(key) {
            case GLFW_KEY_1:
                currentMode3D = TRANSLATE;
                break;
            case GLFW_KEY_2:
                currentMode3D = SCALE;
                break;
            case GLFW_KEY_3:
                currentMode3D = SHEAR;
                break;
            case GLFW_KEY_4:
                currentMode3D = ROTATE;
                break;
            case GLFW_KEY_0:
                currentMode3D = NONE;
                break;
            default:
                break;
        }
    }
}

void drawCube(const Cube& c){
    glBegin(GL_QUADS);
    
    glVertex3f(c.v[0].x, c.v[0].y, c.v[0].z);
    glVertex3f(c.v[1].x, c.v[1].y, c.v[1].z);
    glVertex3f(c.v[4].x, c.v[4].y, c.v[4].z);
    glVertex3f(c.v[2].x, c.v[2].y, c.v[2].z);

    glVertex3f(c.v[0].x, c.v[0].y, c.v[0].z);
    glVertex3f(c.v[2].x, c.v[2].y, c.v[2].z);
    glVertex3f(c.v[6].x, c.v[6].y, c.v[6].z);
    glVertex3f(c.v[3].x, c.v[3].y, c.v[3].z);

    glVertex3f(c.v[0].x, c.v[0].y, c.v[0].z);
    glVertex3f(c.v[1].x, c.v[1].y, c.v[1].z);
    glVertex3f(c.v[5].x, c.v[5].y, c.v[5].z);
    glVertex3f(c.v[3].x, c.v[3].y, c.v[3].z);

    glVertex3f(c.v[1].x, c.v[1].y, c.v[1].z);
    glVertex3f(c.v[4].x, c.v[4].y, c.v[4].z);
    glVertex3f(c.v[7].x, c.v[7].y, c.v[7].z);
    glVertex3f(c.v[5].x, c.v[5].y, c.v[5].z);

    glVertex3f(c.v[2].x, c.v[2].y, c.v[2].z);
    glVertex3f(c.v[4].x, c.v[4].y, c.v[4].z);
    glVertex3f(c.v[7].x, c.v[7].y, c.v[7].z);
    glVertex3f(c.v[6].x, c.v[6].y, c.v[6].z);

    glVertex3f(c.v[3].x, c.v[3].y, c.v[3].z);
    glVertex3f(c.v[5].x, c.v[5].y, c.v[5].z);
    glVertex3f(c.v[7].x, c.v[7].y, c.v[7].z);
    glVertex3f(c.v[6].x, c.v[6].y, c.v[6].z);

    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    for (int i = 0; i < 12; i++) {
        glVertex3fv(&c.v[edges[i][0]].x);
        glVertex3fv(&c.v[edges[i][1]].x);
    }
    glEnd();
}

void drawWireCube(const Cube& c){
    glBegin(GL_LINES);
    for (int i = 0; i < 12; i++) {
        glVertex3fv(&c.v[edges[i][0]].x);
        glVertex3fv(&c.v[edges[i][1]].x);
    }
    glEnd();
}

void transformCube(Cube &c, float M[4][4]){
    for (int i = 0; i < 8; i++) {
        float x = c.v[i].x;
        float y = c.v[i].y;
        float z = c.v[i].z;

        c.v[i].x = M[0][0] * x + M[0][1] * y + M[0][2] * z + M[0][3];
        c.v[i].y = M[1][0] * x + M[1][1] * y + M[1][2] * z + M[1][3];
        c.v[i].z = M[2][0] * x + M[2][1] * y + M[2][2] * z + M[2][3];
    }
}

void translate(Cube c, float tx, float ty, float tz){
    float T[4][4] = {
        {1, 0, 0, tx},
        {0, 1, 0, ty},
        {0, 0, 1, tz},
        {0, 0, 0, 1},
    };

    transformCube(c, T);

    drawWireCube(c);
}

void scale(Cube c, float sx, float sy, float sz){
    float S[4][4] = {
        {sx, 0, 0, 0},
        {0, sy, 0, 0},
        {0, 0, sz, 0},
        {0, 0, 0, 1},
    };

    transformCube(c, S);

    drawWireCube(c);
}

void shear(Cube c, char axis, float shx, float shy, float shz){
    float Sh[4][4] = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1},
    };

    axis = std::toupper(axis);

    if (axis == 'X') {
        Sh[1][0] = shy;
        Sh[2][0] = shz;
    }
    else if (axis == 'Y') {
        Sh[0][1] = shx;
        Sh[2][1] = shz;
    }
    else if (axis == 'Z') {
        Sh[0][2] = shx;
        Sh[1][2] = shy;
    }
    else return;

    transformCube(c, Sh);

    drawWireCube(c);
}

void rotate(Cube c, char axis, float angle){
    float Sh[4][4] = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1},
    };

    float rad = angle * M_PI / 180.0;
    float cosA = cos(rad);
    float sinA = sin(rad);

    axis = std::toupper(axis);

    if (axis == 'X') {
        Sh[1][1] = cosA;
        Sh[1][2] = -sinA;
        Sh[2][1] = sinA;
        Sh[2][2] = cosA;
    }
    else if (axis == 'Y') {
        Sh[0][0] = cosA;
        Sh[0][2] = sinA;
        Sh[2][0] = -sinA;
        Sh[2][2] = cosA;
    }
    else if (axis == 'Z') {
        Sh[0][0] = cosA;
        Sh[0][1] = -sinA;
        Sh[1][0] = sinA;
        Sh[1][1] = cosA;
    }
    else return;

    transformCube(c, Sh);

    drawWireCube(c);
}

int main(){
    if(!glfwInit()) return -1;
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "3D Transform", nullptr, nullptr);
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

    Cube cube = {
        {
            {0.0f, 0.0f, 20.0f},     //0
            {50.0f, 0.0f, 20.0f},    //1
            {0.0f, 50.0f, 20.0f},    //2
            {0.0f, 0.0f, 70.0f},    //3
            {50.0f, 50.0f, 20.0f},   //4
            {50.0f, 0.0f, 70.0f},   //5
            {0.0f, 50.0f, 70.0f},   //6
            {50.0f, 50.0f, 70.0f},  //7
        }
    };
    
    while(!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glLoadIdentity();
        gluLookAt(
            200.0, 200.0, 200.0,
            0.0, 0.0, 0.0,
            0.0, 1.0, 0.0
        );

        drawAxes(150.0f);

        glColor3f(1.0f, 1.0f, 1.0f);
        drawCube(cube);

        glColor3f(0.1f, 0.5f, 0.6f);

        switch(currentMode3D) {
            case TRANSLATE:
                translate(cube, -60.0f, 10.0f, 20.0f);
                break;
            case SCALE:
                scale(cube, 1.5f, 1.5f, 1.5f);
                break;
            case SHEAR:
                shear(cube, 'z', 0.0f, 1.0f, 0.0f);
                break;
            case ROTATE:
                rotate(cube, 'y', 90.0f);
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