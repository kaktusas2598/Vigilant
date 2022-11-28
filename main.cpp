#include <iostream>
#include <string>

#include "INIReader.h"
#include "src/Engine.hpp"

int main(int argc, char *argv[]) {

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
