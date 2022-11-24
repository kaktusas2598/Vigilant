#include "ObjectLayer.hpp"
#include "Entity.hpp"

namespace Vigilant {
    
    // TODO: fix deltaTime param for entities update() and draw() calls
    void ObjectLayer::update() {
        for (int i = 0; i < entities.size(); i++) {
            entities[i]->update(0.0f);
        }
    }

    void ObjectLayer::render() {
        for (int i = 0; i < entities.size(); i++) {
            entities[i]->draw(0.0f);
        }
    }
}