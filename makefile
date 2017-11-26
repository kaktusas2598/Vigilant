#SOURCES
OBJS= main.cpp Demo.cpp GameState.cpp src/*.cpp #src/img/imgui*cpp

#COMPILER
CC= clang++ #Or g++
#COMPILER FLAGS
CF=-Wall -g -std=c++11 -std=c++14 -fexceptions

#LINKER FLAGS
#For Mac OS X: brew install sdl2; brew install sdl2_image; brew install glew; brew install sdl2_ttf
LDF= -framework OpenGL -framework SDL2 -framework SDL2_image -framework SDL2_ttf -lSDL2main -lglew #-lSDL2 -lBox2D -framework SDL2_ttf
#LDF= -framework OpenGL -lglew -lSDL2 -lBox2D -framework SDL2_ttf

INCLUDE_PATHS = -I/usr/local/include -I/opt/X11/include
LIBRARY_PATHS = -L/usr/local/lib -I/opt/X11/lib
OBJ_NAME= main

all : $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(CF) $(LDF) -o $(OBJ_NAME)
