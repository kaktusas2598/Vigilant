#
#  Run 'make' to compile and link
#  Or 'make debug' to compile with debug symbols
#
#SOURCES
OBJS= *.cpp src/*.cpp src/include/imgui/*.cpp
 # src/imgui/imgui*cpp

#COMPILER
CC= g++ # Or clang++
#COMPILER FLAGS
CXXF=-Wall -std=c++11 -std=c++14 -std=c++17 -fexceptions

LDF=

all: executable

# Add -g flag to generate debug symbols on debug build
debug: CXXF += -DDEBUG -g
debug: executable

UNAME := $(shell uname -s)
# For Mac
ifeq ($(UNAME),Darwin)
LDF+= -lSDL2main -lSDL2 -lGL -lGLU -lglut -lGLEW -lm -lSDL2_image -lSDL2_mixer -lSDL2_ttf -llua54 -ld# -lglu32
else ifeq ($(UNAME),Linux)
LDF+= -lSDL2main -lSDL2 -lGL -lGLU -lglut -lGLEW -lm -lSDL2_image -lSDL2_mixer -lSDL2_ttf -llua54# -lglu32
else
# For Windows using MSYS2/MING or Cygwin
LDF+= -lmingw32 -lSDL2main -lSDL2 -mwindows -lglew32  -lopengl32 -lm -lSDL2_image -lSDL2_mixer -lSDL2_ttf -llua54# -lglu32
endif

INCLUDE_PATHS = -I/usr/local/include -I/opt/X11/include -Isrc/include -Isrc/include/imgui
LIBRARY_PATHS = -L/usr/local/lib -L/opt/X11/lib -Lsrc/lib
OBJ_NAME= main

executable : $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(CXXF) $(LDF) -o $(OBJ_NAME)
