OBJS= src/*.cpp
#OBJS2= src/imgui/imgui*cpp

CC= g++
CF=-Wall -g -std=c++11 -std=c++14 -fexceptions

LDF= -framework OpenGL -lglew -lSDL2

INCLUDE_PATHS = -I/usr/local/include -I/opt/X11/include
LIBRARY_PATHS = -L/usr/local/lib -I/opt/X11/lib
OBJ_NAME= main

all : $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(CF) $(LDF) -o $(OBJ_NAME)
