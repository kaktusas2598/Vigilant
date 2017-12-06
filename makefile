#SOURCES
OBJS= *.cpp src/*.cpp src/imgui/imgui*cpp

#COMPILER
CC= clang++ #Or g++
#COMPILER FLAGS
CF=-Wall -g -std=c++11 -std=c++14 -fexceptions

#LINKER FLAGS
#For Mac OS X: brew install sdl2; brew install glew; brew install sdl2_ttf
LDF= -framework OpenGL -framework SDL2 -framework SDL2_ttf -lSDL2main -lglew -lBox2D

INCLUDE_PATHS = -I/usr/local/include -I/opt/X11/include
LIBRARY_PATHS = -L/usr/local/lib -I/opt/X11/lib
OBJ_NAME= main

all : $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(CF) $(LDF) -o $(OBJ_NAME)
