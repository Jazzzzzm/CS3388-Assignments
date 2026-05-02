# initializing libraries
from OpenGL.GL import *
import glfw

# initialize glfw
glfw.init()

# create 1280px by 1000px window with "Hello World" title
window = glfw.create_window(1280, 1000, "Hello World!", None, None)

# set window for rendering
glfw.make_context_current(window)

# setting main colour to purple
glColor3f(0.659, 0.341, 1.0)

# while window is open
while not glfw.window_should_close(window):
    
    # processes mouse, keyboard, and window events
    glfw.poll_events()
    
    # clears screen at start of each frame
    glClear(GL_COLOR_BUFFER_BIT)

    # draw triangle
    glBegin(GL_TRIANGLES)
    glVertex2f(0, 0.5)
    glVertex2f(0.5, -0.25)
    glVertex2f(-0.5, -0.25)
    glEnd()

    # display drawn triangle
    glfw.swap_buffers(window)

# shuts down glfw and clears memory
glfw.terminate()
