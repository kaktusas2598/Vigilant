# Vigilant Engine

2D (Eventually 3D) Game/Physics engine I am working on.

## Project Status

This Project has been abandoned because I am now learning and using OpenGL for rendering instead and that
opened a lot of new possibilities so instead of trying to refactor this project to use OpenGL rendering backend,
I have decided to take best parts from this project, apply what I've learned here and learning OpenGL and start working on
new 2D and 3D! rendering engine here: [OpenGL Engine](https://github.com/kaktusas2598/Villain)

![Vigilant Engine Demo](assets/screenshot.PNG?raw=true "Vigilant Engine Demo: 2D RPG")


#### Supported features:

 * SDL2 2D rendering
 * Finite State Machine and state loading from XML file
 * Entities defined using Lua scripts including Lua listener functions on events
 * Entities can have dynamic behaviours coded using Lua coroutines
 * Keyboard and Mouse input handling with SDL2
 * Window handling with SDL2
 * Texture loading (Using stb_image.h and SDL2_image)
 * TTF Font loading (Using SDL_ttf)
 * Menu buttons with callbacks, labels, backgrounds
 * Sprite animations
 * Basic Audio Support (one shot sfx and music)
 * Tiled map loading from tmx files including collision layers
 * Basis Axis Aligned Collision Detection
 * Basic Rigid Body Dynamics
 * Particle System
 * Debug UI with console for Lua scripting, support history and autocompletion

#### Planned feature list:

 * Multiple levels/ warp zones
 * 3D Rendering with OpenGL
 * Entity Component System
 * Model Loading (simple .obj loader or assimp?)
 * Skeletal animation support
 * Saving/loading
 * Items/inventory


### Used Libraries

 * [OpenGL](https://www.opengl.org) - for 2D rendering and eventually 3D;
 * [SDL2](https://www.libsdl.org/) - for window creation and input handling, sound, optional 2D rendering;
 * [GLEW](https://glew.sourceforge.net/) - extension library for loading OpenGL functions
 * [IMGUI](https://github.com/ocornut/imgui) - Bloat-free Immediate Mode Graphical User interface for C++ with minimal dependencies;
 * [GLM](https://glm.g-truc.net/0.9.8/index.html) - C++ Mathematics library for graphics applications.
 * [TinyXML](https://www.grinninglizard.com/tinyxml/) - For XML file parsing
 + [stb_image](https://github.com/nothings/stb) Public Domain Image Loading Library for C++

 #### Build Notes

 Use the attached makefile to compile and build, but before that make sure your compiler knows where to find libraries and includes

 Outdated Notes:
 To compile Box2D on MacOS Sierra, I copied header files to my /usr/local/include and moved compile static library (.a file) to /usr/local/lib/

### Linux Dependencies

```
sudo apt-get install -y libsdl2-dev
sudo apt-get install -y libsdl2-image-dev
sudo apt-get install -y libsdl2-mixer-dev
sudo apt-get install -y libsdl2-ttf-dev
sudo apt install lua5.4
```

### Building Lua

Visit [lua](://www.lua.org/download.html) homepage for up to date info on how to compile and build Lua
```
curl -R -O http://www.lua.org/ftp/lua-5.4.4.tar.gz
tar zxf lua-5.4.4.tar.gz
cd lua-5.4.4
make all test
```
Or visit [LuaBinaries](https://luabinaries.sourceforge.net/) page and download precompiled libraries
Make sure to remove everything in src/lib before the next step
After compiling find liblua54.a and move it to src/lib directory in Vigilant

### MSYS2 64bit Windows dependencies
```
pacman -S mingw-w64-x86_64-SDL2
pacman -S mingw64/mingw-w64-x86_64-SDL2_mixer
pacman -S mingw64/mingw-w64-x86_64-SDL2_image
pacman -S mingw64/mingw-w64-x86_64-SDL2_ttf
```


## Post-Mortem

I've learned a lot of things so far while working on this project and it still serves as primarily learning
tool for me. I would I could have done some things better and if for some reason it would require to much
work to do them, I will write them down here instead:

 * Instead of binding Entity class to lua using metatable, I could have used lightuserdata and pointers to make memory
   management easier.
 * 2023-01-06: I have been learning and experimenting with Modern OpenGL for the last month in order to convert all rendering to use OpenGL, but after a
   while I realised that it might be a very difficult undertaking so I am thinking about starting a new engine project and forking this one, new engine     would use purely OpenGL for all rendering.

## Posible Additions
 * Rigid body resolution using impulse response
 * Serialization support for saving
 * Warp Zone/tile support
 * Pathfinding behaviours/A*

