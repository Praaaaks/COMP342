#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<windows.h>
#include<GL/glu.h>
#include<iostream>
#include<cmath>

struct Point3D
{
    float x;
    float y;
    float z;
};

struct Pyramid {
    Point3D v[5];
};

// Rotate 45 degrees around Y axis and push forward in z
// cos(45°) ≈ 0.707, sin(45°) ≈ 0.707
Pyramid pyramid = {
    {
        {-70.7f, -100, 100},      // base 0 - rotated, pushed forward
        { 0,     -100, 170.7f},   // base 1 - rotated, pushed forward
        { 70.7f, -100, 100},      // base 2 - rotated, pushed forward
        { 0,     -100, 29.3f},    // base 3 - rotated, pushed forward
        { 0,        0, 100}       // apex 4 - center top, pushed forward
    }
};

bool usePerspective = false;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-200, 200, -200, 200, -500, 500);
}

void drawPoint(Point3D &p){
    glBegin(GL_POINTS);
    glVertex3f(p.x, p.y, p.z);
    glEnd();
}

Point3D orthographicProject(Point3D &p, float vp) {
    Point3D out;
    out.x = p.x;
    out.y = p.y;
    out.z = vp;   // project onto view plane
    return out;
}

Point3D perspectiveProject(Point3D &p, float d){
    // Simple perspective projection formula
    // Projects point onto z=0 plane with viewpoint at z=-d
    Point3D point;
    
    // Avoid division by zero
    float denom = d + p.z;
    if (denom < 0.01f) denom = 0.01f;
    
    float scale = d / denom;
    
    point.x = p.x * scale;
    point.y = p.y * scale;
    point.z = 0;  // Project onto z=0 plane

    return point;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action != GLFW_PRESS) return;

    if (key == GLFW_KEY_P) {
        usePerspective = true;
        std::cout << "Perspective projection\n";
    }
    if (key == GLFW_KEY_O) {
        usePerspective = false;
        std::cout << "Orthographic projection\n";
    }
}

void projectPyramid(const Pyramid& src, Pyramid& dst, bool perspective, float d, float vp) {
    for (int i = 0; i < 5; i++) {
        if (perspective)
            dst.v[i] = perspectiveProject((Point3D&)src.v[i], d);
        else
            dst.v[i] = orthographicProject((Point3D&)src.v[i], vp);
    }
}

void drawPyramidEdges(const Pyramid& p) {
    glColor3f(1.0f, 1.0f, 1.0f);  // White edges
    glBegin(GL_LINES);
    
    // Base edges
    glVertex3f(p.v[0].x, p.v[0].y, p.v[0].z);
    glVertex3f(p.v[1].x, p.v[1].y, p.v[1].z);
    
    glVertex3f(p.v[1].x, p.v[1].y, p.v[1].z);
    glVertex3f(p.v[2].x, p.v[2].y, p.v[2].z);
    
    glVertex3f(p.v[2].x, p.v[2].y, p.v[2].z);
    glVertex3f(p.v[3].x, p.v[3].y, p.v[3].z);
    
    glVertex3f(p.v[3].x, p.v[3].y, p.v[3].z);
    glVertex3f(p.v[0].x, p.v[0].y, p.v[0].z);
    
    // Edges from base to apex
    glVertex3f(p.v[0].x, p.v[0].y, p.v[0].z);
    glVertex3f(p.v[4].x, p.v[4].y, p.v[4].z);
    
    glVertex3f(p.v[1].x, p.v[1].y, p.v[1].z);
    glVertex3f(p.v[4].x, p.v[4].y, p.v[4].z);
    
    glVertex3f(p.v[2].x, p.v[2].y, p.v[2].z);
    glVertex3f(p.v[4].x, p.v[4].y, p.v[4].z);
    
    glVertex3f(p.v[3].x, p.v[3].y, p.v[3].z);
    glVertex3f(p.v[4].x, p.v[4].y, p.v[4].z);
    
    glEnd();
}

void drawSolidPyramid(const Pyramid& p) {
    // Disable backface culling to see all faces
    glDisable(GL_CULL_FACE);
    
    // Set color once before drawing
    glColor3f(0.0f, 0.8f, 1.0f);
    
    // Base (quad) - bottom of pyramid
    glBegin(GL_QUADS);
    glVertex3f(p.v[0].x, p.v[0].y, p.v[0].z);
    glVertex3f(p.v[1].x, p.v[1].y, p.v[1].z);
    glVertex3f(p.v[2].x, p.v[2].y, p.v[2].z);
    glVertex3f(p.v[3].x, p.v[3].y, p.v[3].z);
    glEnd();

    // Sides (triangles)
    glBegin(GL_TRIANGLES);

    // Front-left face (edge 0-1 to apex) - FRONT FACING
    glVertex3f(p.v[0].x, p.v[0].y, p.v[0].z);
    glVertex3f(p.v[1].x, p.v[1].y, p.v[1].z);
    glVertex3f(p.v[4].x, p.v[4].y, p.v[4].z);

    // Front-right face (edge 1-2 to apex)
    glVertex3f(p.v[1].x, p.v[1].y, p.v[1].z);
    glVertex3f(p.v[2].x, p.v[2].y, p.v[2].z);
    glVertex3f(p.v[4].x, p.v[4].y, p.v[4].z);

    // Back-right face (edge 2-3 to apex)
    glVertex3f(p.v[2].x, p.v[2].y, p.v[2].z);
    glVertex3f(p.v[3].x, p.v[3].y, p.v[3].z);
    glVertex3f(p.v[4].x, p.v[4].y, p.v[4].z);

    // Back-left face (edge 3-0 to apex)
    glVertex3f(p.v[3].x, p.v[3].y, p.v[3].z);
    glVertex3f(p.v[0].x, p.v[0].y, p.v[0].z);
    glVertex3f(p.v[4].x, p.v[4].y, p.v[4].z);

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
    glLineWidth(2.0f);  // Make edges more visible
    
    std::cout << "Press 'P' for perspective projection, 'O' for orthographic\n";
    
    while(!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        Pyramid projectedPyramid;

        float d = 100;  // Distance from viewpoint to projection plane
        float vp = 0;   // View plane at z=0

        projectPyramid(pyramid, projectedPyramid, usePerspective, d, vp);

        drawPyramidEdges(projectedPyramid);
        drawSolidPyramid(projectedPyramid);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}