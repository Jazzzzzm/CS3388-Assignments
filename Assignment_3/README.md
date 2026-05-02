# Spline Drawing Tool

## Author: Jasmine Vu

## About
Assignment 3 of class CS3388 - Computer Graphics. This assignment is for students to learn how to code user interaction, splines, and rendering.

With a left-click on the mouse, you can add a node which can also be dragged along the screen. These nodes make up the Bezair curve when 2 or more are present. 
You can control the shape of the curve using the control points connected to the nodes.
To reset the screen, press E on the keyboard to erase all nodes and control points.

## How to Run (Mac)

Downloading glfw:
- brew install gflw

Making Executable and Running Program: 
- g++ spline_draw.cpp -I/opt/homebrew/include -L/opt/homebrew/lib -lglfw -framework OpenGL -o s_draw
- ./s_draw