#SOURCES
OBJS= *.cpp src/*.cpp
 # src/imgui/imgui*cpp

#COMPILER
CC= g++ # Or clang++
#COMPILER FLAGS
CXXF=-Wall -g -std=c++11 -std=c++14 -fexceptions

#LINKER FLAGS
#For Mac OS X: brew install sdl2; brew install glew; brew install sdl2_ttf
# Linux flags LDF= -lSDL2 -lSDL2main -lGL -lGLU -lglut -lGLEW
# For Windows using MSYS2/MINGW64
LDF= -lmingw32 -lSDL2main -lSDL2 -mwindows -lglew32  -lopengl32 -lm -lSDL2_image -lSDL2_mixer -llua54# -lglu32
# Linux
#LDF= -lSDL2main -lSDL2 -lGL -lGLU -lglut -lGLEW -lm -lSDL2_image -lSDL2_mixer # -lglu32

INCLUDE_PATHS = -I/usr/local/include -I/opt/X11/include
LIBRARY_PATHS = -L/usr/local/lib -I/opt/X11/lib
OBJ_NAME= main

all : $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(CXXF) $(LDF) -o $(OBJ_NAME)
