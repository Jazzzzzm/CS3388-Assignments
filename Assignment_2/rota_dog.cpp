#include <GLFW/glfw3.h>
#include <OpenGL/gl.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

// center of circle
int cx = 30, cy = 30, r = 25;

// Function to draw dog and apply transformations to it
void draw_dog(float pos, int angle, vector<float> vertices){

    glPushMatrix();


    // for fun - causes the dogs to go in a circle while also rotating
    //float rad = (pos+angle*0.5) * M_PI / 180.0f;

    float rad = (pos) * M_PI / 180.0f;

    float x = cx + r * cos(rad);
    float y = cy + r * sin(rad);

    glTranslatef(x, y, 0);
    glRotatef(angle, 0, 0, 1);

    glBegin(GL_LINE_STRIP);
        for(int i = 0; i+1< vertices.size(); i+=2){ 
            glVertex2f(vertices[i], vertices[i+1]);
        }
    glEnd();

    glPopMatrix();
}

int main(void)
{
    // Initializing variables
    float x = 30, y = 30, radius = 25;

    float positions[] = {0, 45, 90, 135, 180, 225, 270, 315};

    int angle = 0;

    // Loading dog.txt and getting all data 
    string dog, line;

    ifstream myFile("dog.txt");
    if (!myFile) {
        cerr << "No dog.txt\n";
        return -1;
    }   

    while (getline(myFile, line)) {
        dog += line + " ";
    }

    myFile.close();

    // Splitting string by space and turning separated strings into floats
    vector<float> vertices;
    istringstream iss(dog);
    float v;
    while (iss >> v) {
        vertices.push_back(v);
    }
    
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(750, 750, "dOge", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    //setting up viewing volume
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0, 60, 0, 60, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // setting background colour to white and main colour to black
    glClearColor(1, 1, 1, 0);
    glColor3f(0, 0, 0);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Poll for and process events */
        glfwPollEvents();

		/* Render here */
        
        // Checking for valid angle
        if(angle > 359){
            angle = 0;
        }
 
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Drawing dogs
        for(float pos : positions){
            draw_dog(pos, angle, vertices);
        }
        
        // Rotating one more angle
        angle += 1;

		/* Swap front and back buffers */
        glfwSwapBuffers(window);

    }

    glfwTerminate();
    return 0;
}
