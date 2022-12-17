#include <iostream>
#include <string>

#include "src/Engine.hpp"
#include "src/LuaScript.hpp"

int main(int argc, char *argv[]) {

	Vigilant::LuaScript configScript("scripts/config.lua");
	configScript.open();

	// TODO: read and convert flags to SDL window flags
	unsigned int flags = 0;

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
