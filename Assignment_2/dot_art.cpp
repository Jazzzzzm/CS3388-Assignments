#include <GLFW/glfw3.h>
#include <OpenGL/gl.h>
#include <string>
#include <cstdlib>
#include <iostream>
#include <ctime>

using namespace std;

// Creating Point
struct Point{
    float x, y;
};

// Establishing minimums and maximums
float xmin = -1;
float xmax = 1;
float ymin = -1;
float ymax = 1;

// Initializing global variables
Point p;

Point corners[4] = {
    {xmin, ymin},
    {xmax, ymin},
    {xmax, ymax},
    {xmin, ymax}
};

int prevCorner;

// Function to generate a random float between -1 and 1
static inline double frand(){
    double x = ((double) rand())/(double) RAND_MAX;
    if(rand() % 2){
        x *= -1.0;
    }

    return x;
}

// Function to set up point and corner
void start(){

    //  Setting up program
    p.x = frand();
    p.y = frand();
    prevCorner = rand() % 4;
    return;
}

// Function to draw a dot between a corner and random point
void draw_dot(){

    // Initializing variables
    int randNum = rand() % 3;
    int currCorner;

    // Choosing a non-opposite corner
    if(randNum == 0){
        currCorner = (prevCorner + 3) % 4;
    }
    else if(randNum == 1){
        currCorner = (prevCorner + 1) % 4;
    }
    else{
        currCorner = prevCorner;
    }

    Point c = corners[currCorner];
 
    p.x = (p.x + c.x)/ 2;
    p.y = (p.y + c.y)/ 2;

    // Drawing point
    glVertex2f(p.x, p.y);

    prevCorner = currCorner;
}


int main(int argc, char* argv[])
{
    // Setting random seed
    srand(time(nullptr));

    // Invalid arguments check
    if(argc != 4){
        cout << "Invalid number of arguments";
        exit(0);
    }

    // Initiallizing variables
    int N = atoi(argv[1]);
    int sWidth = atoi(argv[2]); 
    int sHeight = atoi(argv[3]);
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(sWidth, sHeight, "Dot Art", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // Setting up viewing volume
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(-1.1, 1.1, -1.1, 1.1, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Setting background colour to white
    glClearColor(1, 1, 1, 0);

    // Setting colour to black
    glColor3f(0, 0, 0);

    // Setting point size to 2
    glPointSize(2.0f);

    // Clear screen
    glClear(GL_COLOR_BUFFER_BIT);

    // Drawing dot art
    start();

    glBegin(GL_POINTS);

     // Calling draw_dot N times
    for(int i = 0; i<N; i++){
        // Just for fun
        glColor3f(frand(), frand(), 1);

        draw_dot();
    }
    glEnd();

    glfwSwapBuffers(window);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Poll for and process events */
        glfwPollEvents();

    }

    glfwTerminate();
    return 0;
}
