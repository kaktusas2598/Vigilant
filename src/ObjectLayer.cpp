#include "ObjectLayer.hpp"
#include "Entity.hpp"

namespace Vigilant {

    void ObjectLayer::update() {
        for (size_t i = 0; i < entities.size(); i++) {
            entities[i]->update(0.0f);
        }
    }

    void ObjectLayer::render() {
        for (size_t i = 0; i < entities.size(); i++) {
            entities[i]->draw(0.0f);
        }
    }
}
