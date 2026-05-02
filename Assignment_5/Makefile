CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra 

INCLUDES = -I/opt/homebrew/include
LIBPATH  = -L/opt/homebrew/lib
LIBS = -lGLEW -lglfw -framework OpenGL

SRC = main.cpp marching.cpp processing.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = mcubes

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET) $(LIBPATH) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJ)