#include "Level.hpp"

namespace Vigilant {

    void Level::render() {
        for (int i = 0; i < layers.size(); i++) {
            layers[i]->render();
        }
    }

    void Level::update() {
        for (int i = 0; i < layers.size(); i++) {
            layers[i]->update();
        }
    }
}