#include <iostream>
#include <string>

#include "src/Engine.hpp"
#include "src/LuaScript.hpp"

int main(int argc, char *argv[]) {

	Vigilant::LuaScript configScript("scripts/config.lua");
	configScript.open();

	Vigilant::TheEngine::Instance()->init(
		configScript.get<std::string>("window.title"),
		configScript.get<int>("window.height"),
		configScript.get<int>("window.width"),
		0,
		configScript.get<bool>("rendering.SDLRenderingEnabled")
	);
	Vigilant::TheEngine::Instance()->run();

	return 0;
}
