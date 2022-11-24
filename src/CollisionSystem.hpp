#ifndef __CollisionSystem__
#define __CollisionSystem__

#include "ComponentTypes.hpp"
#include "CollisionLayer.hpp"

// TEMP
#include <iostream>

namespace Vigilant {

    class CollisionManager {
        public:
            // TODO: colliders will most likely break once we have a camera in engine
            void checkMapCollision(CollisionLayer* layer) {
                std::vector<SDL_Rect> colliders = layer->getColliders();

                // for (auto const& entity : entities) {
                //     for (auto collider : colliders) {
                //         // Do Collision checks
                //         SDL_Rect entityCollider{
                //             .x = static_cast<int>(transform.x),
                //             .y = static_cast<int>(transform.y),
                //             .w = static_cast<int>(sprite.width),
                //             .h = static_cast<int>(sprite.height)
                //         };
                //         //TODO: apart from resetting entity's velocity, it could also adjust position so that it's not colliding anymore
                //         // because currently it's possible to "ghost" through floors
                //         if (AABB(entityCollider, collider)) {
                //             std::cout << "Map collision!" << std::endl;

                //             // Was travelling downwards, reset position
                //             if (rigidBody.velocityY > 1) {
                //                 transform.y = collider.y - sprite.height;
                //             }

                //             // Infinite bouncing!
                //             // rigidBody.velocityX *= -1;
                //             // rigidBody.velocityY *= -1;
                //             // Reduced bouncing!
                //             rigidBody.velocityX *= -0.8f;
                //             rigidBody.velocityY *= -0.8f;
                //             // No bouncing - let the bodies hit the floor!
                //             // rigidBody.velocityX = 0;
                //             // rigidBody.velocityY = 0;
                //         }
                //     }
                // }
            }

            // TODO: to avoid checking every single collision, look into something like Quad Trees to minimise this
            void checkEntityCollision() {

            }

            void checkEntityProjectileCollision() {
                
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