#include "Level.hpp"

namespace Vigilant {

    Level::~Level() {
	    for(auto& layer : layers) {
			delete layer;
			layer = nullptr;
		}
		layers.clear();
	}

	void Level::render() {
		for (size_t i = 0; i < layers.size(); i++) {
			layers[i]->render();
		}
    }

    void Level::update() {
        for (size_t i = 0; i < layers.size(); i++) {
            layers[i]->update();
        }
    }
}
