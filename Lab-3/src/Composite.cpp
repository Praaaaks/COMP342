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

void reflectY(Matrix3x3 Ry){
    Identity(Ry);
    Ry[0][0] = -1;
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
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "Composite Transform", nullptr, nullptr);
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

        //+90 rotation about (100, 100), followed by uniform scaling by 2 about origin
        Matrix3x3 T1, T2, R, S, result1, result2, result;
        Point r = {100.0f, 100.0f};
        float theta = 90.0f * M_PI / 180.0f;
        translate(T1, -r.x, -r.y);
        rotate(R, theta);
        translate(T2, r.x, r.y);
        scale(S, 2.0f, 2.0f);
        composeTransformations(result1, T1, R);
        composeTransformations(result2, result1, T2);
        composeTransformations(result, result2, S);
        
        glColor3f(0.1f, 0.8f, 0.4f);
        drawTriangle(
            transformPoint(p1, result),
            transformPoint(p2, result),
            transformPoint(p3, result)
        );

        // Reflection about y = mx + c, m=sqrt3, c=-50
        Matrix3x3 T3, T4, R1, R2, Rx, result3, result4, result5, resultR; 
        float m = sqrt(3), c = -50;
        theta = atan(m);
        translate(T3, 0.0f, -c);
        rotate(R1, -theta);
        reflectX(Rx);
        rotate(R2, theta);
        translate(T4, 0.0f, c);
        
        composeTransformations(result3, T3, R1);
        composeTransformations(result4, result3, Rx);
        composeTransformations(result5, result4, R2);
        composeTransformations(resultR, result5, T4);
        
        glColor3f(0.4f, 0.1f, 0.8f);
        drawTriangle(
            transformPoint(p1, resultR),
            transformPoint(p2, resultR),
            transformPoint(p3, resultR)
        );

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}