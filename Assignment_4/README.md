# Rendering Link's house from Ocarina of Time

## Author: Jasmine Vu

## About
This is Assignment 4 from class CS3388 - Computer Graphics. This assignment is for students to learn to build a controllable camera and render models with textures.

### world.cpp
This is the main file where the textures are rendered and the camera is moved using arrow keys.

### readPLYfile.cpp and readPLYfile.h
These files hold the code to read the .ply files. They load vertices, faces, and properties of the textures into memory.

### texturedMesh.cpp and texturedMesh.h
These files hold code for the render pipeline. It loads the data from .ply and .bmp files using functions from readPLYfile.h and LoadBitmap.h. It also creates the vertex shaders and fragment shaders.

### meshData.h
This file contains the structure for VertexData and TriData.

### loadBitmap.cpp
This function was provided by the professor.

## How to Run (Mac)

### Getting house executable
make

### Running program
./house

### Cleaning up executable
make clean

