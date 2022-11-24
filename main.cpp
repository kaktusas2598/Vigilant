#include <iostream>
#include <string>

#include "INIReader.h"
#include "src/Engine.hpp"

// Lua test
#include "src/LuaScript.hpp"

int main(int argc, char *argv[]) {
	
	Vigilant::LuaScript script("scripts/player.lua");
    float posX = script.get<float>("player.transform.X");
    float posY = script.get<float>("player.transform.Y");
    std::string filename = script.get<std::string>("player.sprite.filename");
    int hp = script.get<int>("player.HP");

    std::cout<<"Position X = "<<posX<<", Y = "<<posY<<std::endl;
    std::cout<<"Filename:"<<filename<<std::endl;
    std::cout<<"HP:"<<hp<<std::endl;

	// Load engine config
	INIReader reader("config.ini");
	if (reader.ParseError() != 0) {
		std::cout << "Could not load configuration file!" << std::endl;
	}

	// Initialise and start the engine
	Vigilant::TheEngine::Instance()->init(
		"Vigilant Engine Test",
		reader.GetInteger("rendering", "ScreenHeight", 600),
		reader.GetInteger("rendering", "ScreenWidth", 800),
		0,
		reader.GetBoolean("rendering", "SDLRenderingEnabled", true)
	);
	Vigilant::TheEngine::Instance()->run();

	return 0;
}
