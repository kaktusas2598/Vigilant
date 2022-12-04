#include "Level.hpp"

namespace Vigilant {

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
