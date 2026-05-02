// To ignore experimental warning
#define GLM_ENABLE_EXPERIMENTAL

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream> 
#include <fstream>
#include <sstream>
#include <string>

#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Include header files
#include "texturedMesh.h"

using namespace std; 

// setting camera position, yaw, movement speed, and rotation speed
glm::vec3 camPos = glm::vec3(0.5f, 0.4f, 0.5f);
    float moveSpeed = 0.05f;
    float camYaw = -90.0f;
    float rotaSpeed = 3.0f;

// setting where camera is facing
glm::vec3 camFront = glm::vec3(0.0f, 0.0f, -1.0f);

// setting up user interation with arrow keys
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        if (key == GLFW_KEY_UP) camPos += moveSpeed * camFront;
        if (key == GLFW_KEY_DOWN) camPos -= moveSpeed * camFront;
        if (key == GLFW_KEY_LEFT) camYaw -= rotaSpeed;
        if (key == GLFW_KEY_RIGHT) camYaw += rotaSpeed;

        glm::vec3 front;
        front.x = cos(glm::radians(camYaw));
        front.y = 0.0f;
        front.z = sin(glm::radians(camYaw));
        camFront = glm::normalize(front);
    }
}

int main(void){
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    // setting core for shaders
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1280, 1000, "Wood Cottage", NULL, NULL);
    if (!window){
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

    // enabling depth, alpha, and blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    // setting Key callback
    glfwSetKeyCallback(window, keyCallback);

    // initializing all textures
    TexturedMesh bottles("LinksHouse/Bottles.ply", "LinksHouse/bottles.bmp");
    TexturedMesh curtains("LinksHouse/Curtains.ply", "LinksHouse/curtains.bmp");
    TexturedMesh door("LinksHouse/DoorBG.ply", "LinksHouse/doorbg.bmp");
    TexturedMesh floor("LinksHouse/Floor.ply", "LinksHouse/floor.bmp");
    TexturedMesh metalObj("LinksHouse/MetalObjects.ply", "LinksHouse/metalobjects.bmp");
    TexturedMesh patio("LinksHouse/Patio.ply", "LinksHouse/patio.bmp");
    TexturedMesh table("LinksHouse/Table.ply", "LinksHouse/table.bmp");
    TexturedMesh walls("LinksHouse/Walls.ply", "LinksHouse/walls.bmp");
    TexturedMesh windowBg("LinksHouse/WindowBG.ply", "LinksHouse/windowbg.bmp");
    TexturedMesh woodObj("LinksHouse/WoodObjects.ply", "LinksHouse/woodobjects.bmp");

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Poll for and process events */
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // calculating view
        glm::mat4 view = glm::lookAt(
            camPos, 
            camPos + camFront,
            glm::vec3(0.0f, 1.0f, 0.0f)
        );

        // calculating projection
        glm::mat4 projection = glm::perspective(
            glm::radians(45.0f),
            1280.0f / 1000.0f,
            0.1f,
            100.0f
        );

        // getting model
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.0f, -0.5f));
        model = glm::scale(model, glm::vec3(1.0f));

        // calculating MVP
        glm::mat4 MVP = projection * view * model;

        // rendering opaque objects
        bottles.draw(MVP);
        woodObj.draw(MVP);
        floor.draw(MVP);
        patio.draw(MVP);
        table.draw(MVP);
        walls.draw(MVP);
        windowBg.draw(MVP);

        // rendering transparent objects
        curtains.draw(MVP);
        metalObj.draw(MVP);
        door.draw(MVP);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}