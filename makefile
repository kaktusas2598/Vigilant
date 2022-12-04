#SOURCES
OBJS= *.cpp src/*.cpp src/include/imgui/*.cpp
 # src/imgui/imgui*cpp

#COMPILER
CC= g++ # Or clang++
#COMPILER FLAGS
CXXF=-Wall -std=c++11 -std=c++14 -fexceptions

all: executable

# Add -g flag to generate debug symbols on debug build
debug: CXXF += -DDEBUG -g
debug: executable

# ifeq ($(OS),Windows_NT)
    # For Windows using MSYS2/MING
# LDF= -lmingw32 -lSDL2main -lSDL2 -mwindows -lglew32  -lopengl32 -lm -lSDL2_image -lSDL2_mixer -llua54# -lglu32
# else
    #For Mac OS X: brew install sdl2; brew install glew; brew install sdl2_ttf
    # Linux and MacOS
    # LDF= -lSDL2main -lSDL2 -lGL -lGLU -lglut -lGLEW -lm -lSDL2_image -lSDL2_mixer -llua54# -lglu32
# endif
LDF= -lmingw32 -lSDL2main -lSDL2 -mwindows -lglew32  -lopengl32 -lm -lSDL2_image -lSDL2_mixer -llua54# -lglu32

INCLUDE_PATHS = -I/usr/local/include -I/opt/X11/include -Isrc/include -Isrc/include/imgui
LIBRARY_PATHS = -L/usr/local/lib -L/opt/X11/lib -Lsrc/lib
OBJ_NAME= main

executable : $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(CXXF) $(LDF) -o $(OBJ_NAME)
