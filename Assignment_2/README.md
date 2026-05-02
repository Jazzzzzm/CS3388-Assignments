# Rotating Dog and Dot Art

## Author: Jasmine Vu

## About
Assignment 2 of class CS3388 - Computer Graphics. This assignment is for students to learn simple animation and to use math to draw dot art (Sierpinski's square).

## How to Run (Mac)

Downloading glfw:
- brew install gflw

Rotating Dog: 
- g++ rota_dog.cpp -I/opt/homebrew/include -L/opt/homebrew/lib -lglfw -framework OpenGL -o rota_dog
- ./rota_dog

Dot Art: 
- g++ dot_art.cpp -I/opt/homebrew/include -L/opt/homebrew/lib -lglfw -framework OpenGL -o dot_art
- ./dot_art
