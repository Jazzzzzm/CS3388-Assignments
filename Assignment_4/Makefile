CXX = g++
CXXFLAGS = -std=c++17

INCLUDES = -I/opt/homebrew/include
LIBPATH  = -L/opt/homebrew/lib
LIBS = -lGLEW -lglfw -framework OpenGL

SRC = world.cpp texturedMesh.cpp readPLYfile.cpp LoadBitmap.cpp
TARGET = house

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) \
	$(INCLUDES) $(LIBPATH) $(LIBS)

clean:
	rm -f $(TARGET)