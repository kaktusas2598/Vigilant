# Vigilant Engine

2D (Eventually 3D) Game/Physics engine I am working on.

#### Supported features:

 * Finite State Machine to Load Game States (I would ditch this for Data Driven Design)
 * Keyboard and Mouse input handling with SDL2
 * Window and GL context handling with SDL2
 * GLSL program loading
 * Texture loading (With stb_image.h)
 * Immediate mode GUI using Dear IMGUI (Altrough I am still looking at other options like Nuklear)

#### Planned feature list:

 * Model Loading (simple .obj loader or assimp?)
 * Rigid Body Dynamics support (My Own, not Box2D)
 * Entity-Component Model
 * TTF Font loading
 * Data Driven Design
 * Lua Support (For scripting states and entities?)


### Used Libraries

 * [OpenGL](https://www.opengl.org) - for 2D rendering and eventually 3D;
 * [SDL2](https://www.libsdl.org/) - for window creation and input handling;
 * [IMGUI](https://github.com/ocornut/imgui) - Bloat-free Immediate Mode Graphical User interface for C++ with minimal dependencies;
 * [GLM](https://glm.g-truc.net/0.9.8/index.html) - C++ Mathematics library for graphics applications.
 + stb_image Public Domain Image Loading Library for C++
 * SpriteFont class from Seeds of Andromeda, will implement my own, as I can't get it working at the moment.

 #### Build Notes

 To compile Box2D on MacOS Sierra, I copied header files to my /usr/local/include and moved compile static library (.a file) to /usr/local/lib/
