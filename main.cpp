#include <iostream>
#include <string>

#include "src/Engine.hpp"
#include "src/LuaScript.hpp"

int main(int argc, char *argv[]) {

	Vigilant::LuaScript configScript("scripts/config.lua");
	configScript.open();

	unsigned int flags = 0;
        if (configScript.get<bool>("window.fullscreen"))
            flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
        if (configScript.get<bool>("window.borderless"))
            flags |= SDL_WINDOW_BORDERLESS;
        if (configScript.get<bool>("window.resizable"))
            flags |= SDL_WINDOW_RESIZABLE;

	Vigilant::TheEngine::Instance()->init(
		configScript.get<std::string>("window.title"),
		configScript.get<int>("window.height"),
		configScript.get<int>("window.width"),
		flags,
		configScript.get<bool>("rendering.SDLRenderingEnabled")
	);
	Vigilant::TheEngine::Instance()->run();

	return 0;
}
