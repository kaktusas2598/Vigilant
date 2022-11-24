# Vigilant Engine

2D (Eventually 3D) Game/Physics engine I am working on.

#### Supported features:

 * SDL2 2D rendering (OpenGL support will be coming later)
 * Finite State Machine and state loading from XML file
 * Keyboard and Mouse input handling with SDL2
 * Window and GL context handling with SDL2
 * GLSL shader program loading
 * Texture loading (With stb_image.h and SDL2_image)
 * Menu buttons with callbacks
 * Basic Audio Support (one shot sfx and music)
 * Tiled map loading from tmx files including collision layers

#### Planned feature list:

 * Multiple animation support
 * 3D Rendering with OpenGL
 * Entity Component System
 * Model Loading (simple .obj loader or assimp?)
 * Rigid Body Dynamics support
 * TTF Font loading
 * Data Driven Design
 * Lua Support (For scripting states and entities)
 * Immediate mode GUI using Dear IMGUI (Altrough I am still looking at other options like Nuklear)


### Used Libraries

 * [OpenGL](https://www.opengl.org) - for 2D rendering and eventually 3D;
 * [SDL2](https://www.libsdl.org/) - for window creation and input handling, optional 2D rendering;
 * [GLEW](https://glew.sourceforge.net/) - extension library for loading OpenGL functions
 * [IMGUI](https://github.com/ocornut/imgui) - Bloat-free Immediate Mode Graphical User interface for C++ with minimal dependencies;
 * [GLM](https://glm.g-truc.net/0.9.8/index.html) - C++ Mathematics library for graphics applications.
 * [TinyXML](https://www.grinninglizard.com/tinyxml/) - For XML file parsing
 * [inih](https://github.com/jtilly/inih) - header only C++ version of inih - ini file loader and parser
 + stb_image Public Domain Image Loading Library for C++

 #### Build Notes

 Use the attached makefile to compile and build, but before that make sure your compiler knows where to find libraries and includes

 Outdated Notes:
 To compile Box2D on MacOS Sierra, I copied header files to my /usr/local/include and moved compile static library (.a file) to /usr/local/lib/
