#define GLM_ENABLE_EXPERIMENTAL

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream> 
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>

#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Include functions
#include "marching.h"
#include "processing.h"

using namespace std; 

// setting camera position, r, phi, theta, and sensitivity

float r = sqrt(75);
float theta = 45 * (M_PI/180.0f);
float phi = 54.7 * (M_PI/180.0f);
float sense = 0.05f;

double lastX, lastY;
bool isDrag = false;

glm::vec3 camPos;
glm::mat4 view;

// setting up mouse dragging 
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
    if (button == GLFW_MOUSE_BUTTON_LEFT){
        if (action == GLFW_PRESS){
            isDrag = true;
            glfwGetCursorPos(window, &lastX, &lastY);
        }
        else if (action == GLFW_RELEASE){
            isDrag = false;
        }
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos){
    if (!isDrag) return;

    double dx = xpos - lastX;
    double dy = ypos - lastY;

    lastX = xpos;
    lastY = ypos;

    theta += dx * sense;
    phi   -= dy * sense;

    // prevent flipping
    if(phi < 0.1f) phi = 0.1f;
    if(phi > 3.13f) phi = 3.13f;
}

// setting up arrow keys to change r
void processKeys(GLFWwindow *window){
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
        r -= 0.05f;
        if(r < 1.0f) r = 1.0f;
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
        r += 0.05f;
    }
}

vector<float> createBoundingBox(float min, float max){
    vector<float> v ={
        min,min,min,  max,min,min,
        max,min,min,  max,max,min,
        max,max,min,  min,max,min,
        min,max,min,  min,min,min,

        min,min,max,  max,min,max,
        max,min,max,  max,max,max,
        max,max,max,  min,max,max,
        min,max,max,  min,min,max,

        min,min,min,  min,min,max,
        max,min,min,  max,min,max,
        max,max,min,  max,max,max,
        min,max,min,  min,max,max
    };

    return v;
}

vector<float> createAxes(float min, float max){
    float L = max - min;
    return {
        // X axis (Red)
        min, min, min,  1.0f, 0.0f, 0.0f,
        min+L, min, min,  1.0f, 0.0f, 0.0f,

        // Y axis (Green)
        min, min, min,  0.0f, 1.0f, 0.0f,
        min, min+L, min,  0.0f, 1.0f, 0.0f,

        // Z axis (Blue)
        min, min, min,  0.0f, 0.0f, 1.0f,
        min, min, min+L,  0.0f, 0.0f, 1.0f
    };
}

float sheet(float x, float y, float z){
    return y - sin(x)*cos(z);
}

float cone(float x, float y, float z){
    return x*x - y*y - z*z -z;
}

float iso;
float min1 = -5.0f;
float max1 = 5.0f;
float step = 0.1f;

int main(void)
{
    // Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    float screenW = 1400;
	float screenH = 900;
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(screenW, screenH, "Cool Meshes", NULL, NULL);
    if (!window)
    {
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

    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    string VertexShaderCode = "\
        #version 410 core\n\
        layout(location=0) in vec3 position;\n\
        layout(location=1) in vec3 normal;\n\
        layout(location=2) in vec3 colorIn;\n\
        uniform mat4 MVP;\n\
        uniform mat4 V;\n\
        out vec3 N;\n\
        out vec3 L;\n\
        out vec3 Vdir;\n\
        out vec3 vertexColor;\n\
        uniform vec3 LightDir;\n\
        void main() {\n\
            gl_Position = MVP * vec4(position,1.0);\n\
            vec3 posView = vec3(V * vec4(position,1.0));\n\
            N = normalize(mat3(V) * normal);\n\
            L = normalize(LightDir);\n\
            Vdir = normalize(-posView);\n\
            vertexColor = colorIn;\n\
        }\n";

    string FragmentShaderCode = "\
        #version 410 core\n\
        in vec3 N;\n\
        in vec3 L;\n\
        in vec3 Vdir;\n\
        in vec3 vertexColor;\n\
        uniform vec3 modelColor;\n\
        uniform bool drawingAxes;\n\
        out vec4 fragColor;\n\
        void main(){\n\
            if(drawingAxes){\n\
                fragColor = vec4(vertexColor, 1.0);\n\
            } else {\n\
                vec3 ambient = vec3(0.2,0.2,0.2);\n\
                float diff = max(dot(N,L),0.0);\n\
                vec3 diffuse = diff * modelColor;\n\
                vec3 R = reflect(-L,N);\n\
                float spec = pow(max(dot(R,Vdir),0.0),64.0);\n\
                vec3 specular = spec * vec3(1.0,1.0,1.0);\n\
                fragColor = vec4(ambient + diffuse + specular, 1.0);\n\
            }\n\
        }\n";

    GLint Result = GL_FALSE;
    int InfoLogLength;
    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        printf("%s\n", &VertexShaderErrorMessage[0]);
    }

    const char* VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
    glCompileShader(VertexShaderID);

    // Compile Fragment Shader
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
    glCompileShader(FragmentShaderID);

    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    GLint MVPLoc = glGetUniformLocation(ProgramID, "MVP");
    GLint VLoc   = glGetUniformLocation(ProgramID, "V");
    GLint LightDirLoc = glGetUniformLocation(ProgramID, "LightDir");
    GLint ColorLoc = glGetUniformLocation(ProgramID, "modelColor");

    glEnable(GL_DEPTH_TEST);

    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    
    // for first example
    // iso = 0;
    // vector<float> vertices = marching_cubes(sheet, iso, min1, max1, step);

    // for second example
    iso = -1.5f;
    vector<float> vertices = marching_cubes(cone, iso, min1, max1, step);

    vector<float> normals = compute_normals(vertices);

    // for fist example 
    //writePLY(vertices, normals, "example_1.ply");

    // for second example
    writePLY(vertices, normals, "example_2.ply"); 

    GLuint VAO, VBO[2];

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(2, VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER,
                vertices.size()*sizeof(float),
                vertices.data(),
                GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER,
                normals.size()*sizeof(float),
                normals.data(),
                GL_STATIC_DRAW);

    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,(void*)0);
    glEnableVertexAttribArray(1);

    vector<float> boxVerts = createBoundingBox(min1, max1);
    vector<float> axesVerts = createAxes(min1, max1);

    GLuint boxVAO, boxVBO;
    glGenVertexArrays(1, &boxVAO);
    glGenBuffers(1, &boxVBO);
    glBindVertexArray(boxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, boxVBO);
    glBufferData(GL_ARRAY_BUFFER, boxVerts.size() * sizeof(float), boxVerts.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    GLuint axesVAO, axesVBO;
    glGenVertexArrays(1, &axesVAO);
    glGenBuffers(1, &axesVBO);
    glBindVertexArray(axesVAO);
    glBindBuffer(GL_ARRAY_BUFFER, axesVBO);
    glBufferData(GL_ARRAY_BUFFER, axesVerts.size() * sizeof(float), axesVerts.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    GLint AxesLoc = glGetUniformLocation(ProgramID, "drawingAxes");
    glBindVertexArray(axesVAO);
    glBindBuffer(GL_ARRAY_BUFFER, axesVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Poll for and process events */
        glfwPollEvents();

        processKeys(window);
        
		/* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camPos.x = r * sin(phi) * cos(theta);
        camPos.y = r * cos(phi);
        camPos.z = r * sin(phi) * sin(theta);

        view = glm::lookAt(
            camPos,
            glm::vec3(0.0f,0.0f,0.0f),   // look at origin
            glm::vec3(0.0f,1.0f,0.0f)    // up direction
        );

        glm::mat4 projection = glm::perspective(
            glm::radians(45.0f),
            1280.0f/1000.0f,
            0.1f,
            100.0f
        );

        glm::mat4 model = glm::mat4(1.0f);

        glm::mat4 MVP = projection * view * model;

        glUseProgram(ProgramID);
        glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, glm::value_ptr(MVP));
        glUniformMatrix4fv(VLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniform3f(LightDirLoc, 0.5f, 1.0f, 0.5f);

        // 1. Draw Axes
        glUniform1i(AxesLoc, 1); 
        glBindVertexArray(axesVAO);
        glDrawArrays(GL_LINES, 0, 6); 

        // 2. Draw Bounding Box (Uses modelColor)
        glUniform1i(AxesLoc, 0); 
        glUniform3f(ColorLoc, 0.5f, 0.5f, 0.5f); 
        glBindVertexArray(boxVAO);
        glDrawArrays(GL_LINES, 0, boxVerts.size() / 3);

        // 3. Draw Mesh
        glUniform3f(ColorLoc, 0.5f, 0.0f, 0.6f); 
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);
        
		/* Swap front and back buffers */
        glfwSwapBuffers(window);

    }

    glfwTerminate();
    return 0;
}
