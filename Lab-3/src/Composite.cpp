#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<cmath>

struct Point
{
    float x;
    float y;
};

float sx, sy;

typedef float Matrix3x3[3][3];

void Identity(Matrix3x3 M){
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            M[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    }
}

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

void translate(Matrix3x3 T, float tx, float ty){
    Identity(T);
    T[0][2] = tx;
    T[1][2] = ty;
}

void scale(Matrix3x3 S, float sx, float sy){
    Identity(S);
    S[0][0] = sx;
    S[1][1] = sy;
}

void rotate(Matrix3x3 R, float theta){
    Identity(R);
    R[0][0] = cos(theta);
    R[0][1] = -sin(theta);
    R[1][0] = sin(theta);
    R[1][1] = cos(theta);

}

void shear(Matrix3x3 Sh, float shx, float shy){
    Identity(Sh);
    Sh[0][1] = shx;
    Sh[1][0] = shy;
}

void reflectX(Matrix3x3 Rx){
    Identity(Rx);
    Rx[1][1] = -1;
}

void matrixMultiply(Matrix3x3 A, Matrix3x3 B, Matrix3x3 result){
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            result[i][j] = 0;
            for(int k = 0; k < 3; k++){
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void composeTransformations(Matrix3x3 result, Matrix3x3 T1, Matrix3x3 T2){
    matrixMultiply(T2, T1, result);
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

        glColor3f(1.0f, 1.0f, 1.0f);
        drawAxes();

        Point p1 = {-50.0f, 50.0f};
        Point p2 = {0.0f, 100.0f};
        Point p3 = {50.0f, 50.0f};

        glColor3f(0.8f, 0.1f, 0.4f);
        drawTriangle(p1, p2, p3);

        //+90 rotation about (100, 100)
        glColor3f(0.1f, 0.8f, 0.4f);
        Matrix3x3 T1, T2, R, result1, result2;
        translate(T1, -100.0f, -100.0f);
        rotate(R, 90.0f * M_PI / 180.0f);
        translate(T2, 100.0f, 100.0f);
        composeTransformations(result1, T1, R);
        composeTransformations(result2, result1, T2);

        drawTriangle(
            transformPoint(p1, result2),
            transformPoint(p2, result2),
            transformPoint(p3, result2)
        );

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}