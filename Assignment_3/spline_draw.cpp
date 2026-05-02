// Initializing libraries
#include <GLFW/glfw3.h>
#include <OpenGL/gl.h>
#include <vector>
#include <iostream>
#include <cmath>
#include <cstdlib>

using namespace std;

// Declaring Structures and new Data Type

// struture point
struct Point{
    float x;
    float y;
};

// structure node  
struct Node : Point{
    bool hasHandle1, hasHandle2;
    Point handle1;
    Point handle2;
};

// enum to check if user drags a node or control point
enum DragType{
    NONE,
    DRAG_NODE,
    DRAG_HANDLE1,
    DRAG_HANDLE2
};

// struture to track distance dragged
struct DragState {
    DragType type = NONE;
    int nodeIndex = -1;

    float xoffset, yoffset;
};

// vector of nodes to keep track of nodes
vector<Node> nodes;

// initializing a drag state
DragState drag;

// function to check if a node/control point is 
bool hit(Point one, Point two, float r = 8.0f){
    float hitx = one.x - two.x;
    float hity = one.y - two.y;
    if((hitx*hitx + hity*hity) <= r*r){
        return true;
    }
    return false;
}

// funciton to trigger drag state and calculate offset
void moveNode(Point mouse){
    for(int i = 0; i<nodes.size(); i++){
        if(hit(nodes[i], mouse)){
            drag.type = DRAG_NODE;
            drag.nodeIndex = i;
            drag.xoffset = nodes[i].x - mouse.x;
            drag.yoffset = nodes[i].y - mouse.y;
            return;
        }
    }
}

// function move node and control points
void dragNode(Point mouse) {
    if (drag.type == DRAG_NODE) {
        Node& n = nodes[drag.nodeIndex];

        float newX = mouse.x + drag.xoffset;
        float newY = mouse.y + drag.yoffset;

        float dx = newX - n.x;
        float dy = newY - n.y;

        n.x = newX;
        n.y = newY;

        if (n.hasHandle1) {
            n.handle1.x += dx;
            n.handle1.y += dy;
        }
        if (n.hasHandle2) {
            n.handle2.x += dx;
            n.handle2.y += dy;
        }
    }
}

// function to check for which control point is being dragged
void startHandleDrag(Point mouse){
    for (int i = 0; i < nodes.size(); i++) {
        Node& n = nodes[i];

        if (n.hasHandle1 && hit(n.handle1, mouse, 8.0f)) {
            drag.type = DRAG_HANDLE1;
            drag.nodeIndex = i;
            return;
        }
        if (n.hasHandle2 && hit(n.handle2, mouse, 8.0f)) {
            drag.type = DRAG_HANDLE2;
            drag.nodeIndex = i;
            return;
        }
    }
}

// helper function to move opposite control point
void dragHandle(Node& n, Point& moved, Point& other, Point mouse) {
    moved = mouse;

    float vx = moved.x - n.x;
    float vy = moved.y - n.y;

    other.x = n.x - vx;
    other.y = n.y - vy;
}

// function to move control point
void moveCtrlPt(Point mouse) {
    if (drag.type == DRAG_HANDLE1) {
        Node& n = nodes[drag.nodeIndex];
        if (n.hasHandle2)
            dragHandle(n, n.handle1, n.handle2, mouse);
        else
            n.handle1 = mouse;
    }

    if (drag.type == DRAG_HANDLE2) {
        Node& n = nodes[drag.nodeIndex];
        if (n.hasHandle1)
            dragHandle(n, n.handle2, n.handle1, mouse);
        else
            n.handle2 = mouse;
    }
}

// boolean to check if left click is pressed and held
bool leftPressed = false;

// function to keep track of mouse interaction
void mouseInteraction(GLFWwindow* window, int action) {

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    int w, h;
    glfwGetWindowSize(window, &w, &h);

    Point mouse = { (float)xpos, (float)(h - ypos) };

    // mouse press
    if (action == GLFW_PRESS && !leftPressed) {
        leftPressed = true;
        drag.type = NONE;

        startHandleDrag(mouse);
        if (drag.type != NONE) return;

        moveNode(mouse);
        if (drag.type != NONE) return;

        // add new node
        Node newNode;
        newNode.x = mouse.x;
        newNode.y = mouse.y;
        newNode.hasHandle1 = true;
        newNode.hasHandle2 = false;
        newNode.handle1.x = mouse.x;
        newNode.handle1.y = mouse.y - 50;

        if (nodes.empty()){
            nodes.push_back(newNode);
            return;
        }

        float dStart = hypot(mouse.x - nodes.front().x,
                             mouse.y - nodes.front().y);
        float dEnd = hypot(mouse.x - nodes.back().x,
                           mouse.y - nodes.back().y);

        if (dStart < dEnd) {
            Node& old = nodes.front();
            old.hasHandle2 = true;

            float vx = old.handle1.x - old.x;
            float vy = old.handle1.y - old.y;
            old.handle2.x = old.x - vx;
            old.handle2.y = old.y - vy;

            nodes.insert(nodes.begin(), newNode);
        } 
        else{
            Node& old = nodes.back();
            old.hasHandle2 = true;

            float vx = old.handle1.x - old.x;
            float vy = old.handle1.y - old.y;
            old.handle2.x = old.x - vx;
            old.handle2.y = old.y - vy;

            nodes.push_back(newNode);
        }
    }

    // mouse release
    if (action == GLFW_RELEASE && leftPressed){
        leftPressed = false;
        drag.type = NONE;
    }
}

// getting cursor position
void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos){
    if (!leftPressed) return;

    int w, h;
    glfwGetWindowSize(window, &w, &h);

    Point mouse = { (float)xpos, (float)(h - ypos) };

    if (drag.type == DRAG_NODE) {
        dragNode(mouse);
    } else if (drag.type == DRAG_HANDLE1 || drag.type == DRAG_HANDLE2) {
        moveCtrlPt(mouse);
    }
}

// function mouse buttson callback
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT)
        mouseInteraction(window, action);
}

// function for e key
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_E && action == GLFW_PRESS)
        nodes.clear();
}

// function to calculate each point on bezier curve
Point bezier(Point p0, Point p1, Point p2, Point p3, float t) {
    float u = 1.0f - t;

    Point p;
    p.x = u*u*u*p0.x +
          3*u*u*t*p1.x +
          3*u*t*t*p2.x +
          t*t*t*p3.x;

    p.y = u*u*u*p0.y +
          3*u*u*t*p1.y +
          3*u*t*t*p2.y +
          t*t*t*p3.y;

    return p;
}

// function to draw bezier cruve
void drawBezierSpline() {
    if (nodes.size() < 2) return;

    glColor3f(0, 0, 0);
    glLineWidth(2.0f);

    glBegin(GL_LINE_STRIP);

    for (int i = 0; i < nodes.size() - 1; i++) {
        Node& a = nodes[i];
        Node& b = nodes[i + 1];

        Point p0 = a;
        Point p1 = a.hasHandle2 ? a.handle2 : a;
        Point p2 = b.hasHandle1 ? b.handle1 : b;
        Point p3 = b;

        const int STEPS = 30;
        for (int j = 0; j <= STEPS; j++) {
            float t = (float)j / STEPS;
            Point p = bezier(p0, p1, p2, p3, t);
            glVertex2f(p.x, p.y);
        }
    }

    glEnd();
}

// function to draw point
void drawPoint(Point p, float size = 8.0f){
    glPointSize(size);
    glBegin(GL_POINTS);
    glVertex2f(p.x, p.y);
    glEnd();
}

// function to draw control points 
void drawControlStructure() {
    for (auto& n : nodes) {

        // Node
        glColor3f(1, 0, 0);
        drawPoint(n, 6);

        // Handles
        glColor3f(0, 0, 1);
        if (n.hasHandle1) {
            glBegin(GL_LINES);
            glVertex2f(n.x, n.y);
            glVertex2f(n.handle1.x, n.handle1.y);
            glEnd();
            drawPoint(n.handle1, 4);
        }

        if (n.hasHandle2) {
            glBegin(GL_LINES);
            glVertex2f(n.x, n.y);
            glVertex2f(n.handle2.x, n.handle2.y);
            glEnd();
            drawPoint(n.handle2, 4);
        }
    }
}

int main(int argc, char* argv[]){
    
    // check if right amount of
    if(argc != 3){
        cout << "Invalid number of arguments";
        exit(0);
    }

    // getting width and height of window
    int width = atoi(argv[1]);
    int height = atoi(argv[2]);
    
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "A Spline Tool", NULL, NULL);
    if (!window)
    { 
        glfwTerminate();
        return -1;
    }

    // setting up keys
    glfwMakeContextCurrent(window);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPositionCallback);

    glfwSetKeyCallback(window, keyCallback);

    // setting up viewport
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1, 1);

    glViewport(0, 0, width, height);

    // enabling multisampling
    glEnable(GL_MULTISAMPLE);

    // enabling smooth point for circles
    glEnable(GL_POINT_SMOOTH);


    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glClearColor(1, 1, 1, 0);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Poll for and process events */
        glfwPollEvents();

		/* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        drawBezierSpline();
        drawControlStructure();

		/* Swap front and back buffers */
        glfwSwapBuffers(window);

    }

    glfwTerminate();
    return 0;
}
