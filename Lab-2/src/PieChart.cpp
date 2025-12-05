#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<cmath>

struct Slice
{
    float startAngle;
    float endAngle;
    float r, g, b;
};

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

void lineDDA(int x1, int y1, int x2, int y2){
    int dx = x2 - x1;
    int dy = y2 - y1;

    int stepsize = fabs(dy) > fabs(dx) ? fabs(dy) : fabs(dx);
    float xInc = (float)dx / stepsize;
    float yInc = (float)dy / stepsize;

    float x = (float)x1;
    float y = (float)y1;

    for(int i = 0; i <= stepsize; i++){
        drawPoint((float)round(x), (float)round(y));
        // std::cout<<"Point ("<<x<<", "<<y<<") = Point ("<<round(x)<<", "<<round(y)<<")\n";
        x += xInc;
        y += yInc;
    }
}

bool inSlice(float deg, float start, float end){
    return deg >=start && deg < end;
}

void draw8points(int x, int xc, int y, int yc, Slice slices[], int numSlices){
    int px[8] = {
        x + xc, -x + xc, x + xc, -x + xc,
        y + xc, -y + xc, y + xc, -y + xc
    };

    int py[8] = {
        y + yc,  y + yc, -y + yc, -y + yc,
        x + yc,  x + yc, -x + yc, -x + yc
    };

    for(int i = 0; i < 8; i++){
        float angle = atan2(py[i] - yc, px[i] - xc) * 180 / 3.14159;
        if(angle < 0) angle += 360;

        for(int s = 0; s < numSlices; s++){
            if(inSlice(angle, slices[s].startAngle, slices[s].endAngle)){
                glColor3f(slices[s].r, slices[s].g, slices[s].b);
                lineDDA(xc, yc, px[i], py[i]);    // fill the slice
            }
        }
    }
}

void midpointCircle(int xc, int yc, int r, Slice slices[], int numSlices){
    int x = 0, y = r;
    int p = 1 - r;

    while (x < y)
    {
        draw8points(x, xc, y, yc, slices, numSlices);
        if(p < 0){
            x++;
            p += 2 * x + 1;
        } else {
            x++;
            y--;
            p += 2 * x - 2 * y + 1;
        }
    }
}

int main(){
    if(!glfwInit()) return -1;
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "Circle", nullptr, nullptr);
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

    //Input dataset
    int data[] = {20, 25, 40, 80};
    int numSlices = sizeof(data) / sizeof(data[0]);

    int total = 0;
    for(int i = 0; i < numSlices; i++){
        total += data[i];
    }

    //Calculate angles of each data
    float angle[numSlices];
    for(int i = 0; i < numSlices; i++){
        angle[i] = (float)data[i] / total * 360;
    }

    //Set the starting and ending points of each slice
    Slice slices[numSlices];
    slices[0].startAngle = 0.0;
    slices[0].endAngle = angle[0];
    for(int i = 1; i < numSlices; i++){
        slices[i].startAngle =  slices[i-1].endAngle;
        slices[i].endAngle = slices[i].startAngle + angle[i];
    }

    //Select color for each slice
    for(int i = 0; i < numSlices; i++){
        slices[i].r = abs(1 - (0.1 + i * 0.1));
        slices[i].g = abs(1 - (0.4 + i * 0.2));
        slices[i].b = abs(1 - (0.7 + i * 0.4));
    }
    
    while(!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT);

        glColor3f(0.2f, 0.25f, 0.8f);
        midpointCircle(0, 0, 150, slices, numSlices);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}