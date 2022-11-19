#ifndef __CollisionSystem__
#define __CollisionSystem__

#include "ECS.hpp"
#include "Coordinator.hpp"
#include "ComponentTypes.hpp"
#include "CollisionLayer.hpp"

// TEMP
#include <iostream>

namespace Vigilant {

    class CollisionSystem : public System {
        public:
            // TODO: colliders will most likely break once we have a camera in engine
            void checkMapCollision(CollisionLayer* layer) {
                std::vector<SDL_Rect> colliders = layer->getColliders();

                for (auto const& entity : entities) {
                    auto& transform = TheCoordinator::Instance()->getComponent<Transform>(entity);
                    auto& rigidBody = TheCoordinator::Instance()->getComponent<RigidBody>(entity);
                    auto& sprite = TheCoordinator::Instance()->getComponent<Sprite>(entity);

                    
                    for (auto collider : colliders) {
                        // Do Collision checks
                        SDL_Rect entityCollider{
                            .x = static_cast<int>(transform.x),
                            .y = static_cast<int>(transform.y),
                            .w = static_cast<int>(sprite.width),
                            .h = static_cast<int>(sprite.height)
                        };
                        if (AABB(entityCollider, collider)) {
                            std::cout << "Map collision!" << std::endl;
                            rigidBody.velocityX *= -1;
                            rigidBody.velocityY *= -1;
                        }
                    }
                }
            }

            // TODO: to avoid checking every single collision, look into something like Quad Trees to minimise this
            void checkEntityCollision() {

            }
        private:
            // Simple Axis-Alligned Bounding Box collision
            bool AABB(SDL_Rect a, SDL_Rect b) {
                int leftA, leftB;
                int rightA, rightB;
                int topA, topB;
                int bottomA, bottomB;

                leftA = a.x;
                rightA = a.x + a.w;
                topA = a.y;
                bottomA = a.y + a.h;

                leftB = b.x;
                rightB = b.x + b.w;
                topB = b.y;
                bottomB = b.y + b.h;

                // If any of the sides from A are outside of B
                if (bottomA <= topB) { return false; }
                if (topA >= bottomB) { return false; }
                if (rightA <= leftB) { return false; }
                if (leftA >= rightB) { return false; }
                
                return true;
            }
    };
}

#endif // __CollisionSystem__