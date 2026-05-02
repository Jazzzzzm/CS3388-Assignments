# Surface Generated Using Marching Cubes

## Author: Jasmine Vu

## About
This is Assignment 5 from class CS3388 - Computer Graphics. This assignment is for students to learn to implement marching cubes algorithm and render using Phong shaders.

## File Overview
### main.cpp
This is the main file where camera is moved using mouse and arrow key interactions and where the shaders are created and used.

### marching.cpp and marching.h
These files hold the function to calculate marching squares using the provided marching cube lookup table, generate triangle vertices for triangle mesh, and return a float vector of those vertices.

### processing.cpp and processing.h
These files hold the function to calculate the normals from the results of marching_cubes and to generate a .ply file with those vertices. 

For compute_normals(), it takes the result from matching_cubes() and calculates the normals in counter-clockwise winding.

For writePLY(), it takes the result from matching_cubes() and compute_normals() and puts all the triangle vertices into a .ply file with a specified file name.

## Example Surfaces
You can manually uncomment each example in main.cpp (lines 255-270). The first example generates a sheet of waves and the second example generates an hourglass like structure.

## Controls

### Mouse
- Left Mouse Drag: Rotate the camera around the object

### Keyboard
- Up Arrow: Zoom in
- Down Arrow: Zoom out

## How to Run (Mac)

### Library requirements
- OpenGL
- GLFW
- GLEW
- GLM

### Getting house executable
make

### Running program
./mcubes

### Cleaning up executables
make clean

