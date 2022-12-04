#ifndef __Collision__
#define __Collision__

#include <string>

#include "CollisionLayer.hpp"

#include "TileLayer.hpp"
#include "Vector2D.hpp"

#include "PhysicsComponent.hpp"
#include "SpriteComponent.hpp"

namespace Vigilant {

    class Collision {
        public:
            // Simple Axis-Alligned Bounding Box collision
            static bool AABB(SDL_Rect& a, SDL_Rect& b) {
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

            static void checkMapCollision(Entity* entity, const std::vector<TileLayer*>& collisionLayers) {
                // Check if entity has required components
                // TODO sprite should not be needed
                auto physics = entity->getComponent<PhysicsComponent>();
                auto sprite = entity->getComponent<SpriteComponent>();
                if (physics == 0 && sprite == 0) {
                    return;
                }
                
                for(std::vector<TileLayer*>::const_iterator it = collisionLayers.begin(); it != collisionLayers.end(); ++it) {
                    TileLayer* tileLayer = (*it);
                    std::vector<std::vector<int>> tiles = tileLayer->getTileIDs();

                    Vector2D layerPos = tileLayer->getPosition();
                    int x, y, tileColumn, tileRow, tileid = 0;

                    x = layerPos.getX() / tileLayer->getTileSize();
                    y = layerPos.getY() / tileLayer->getTileSize();

                    float velocityX = physics->getVelocityX();
                    float velocityY = physics->getVelocityY();
                    float entityX = entity->transform->getX();
                    float entityY = entity->transform->getY();
                    int width = sprite->getWidth() * entity->transform->getScaleX();
                    int height = sprite->getHeight() * entity->transform->getScaleY();

                    // FIXME: seg fault when going from the map (tileid cannot be deduced)
                    // also camera is not accounted here which cause inaccurate collision
                    // TheEngine::Instance()->camera.x = gameEntities[i]->transform->getX() - TheEngine::Instance()->camera.w/2;
                    // TheEngine::Instance()->camera.y = gameEntities[i]->transform->getY() - TheEngine::Instance()->camera.h/2;
                    if (entityX > 0 && entityY > 0) {
                        if (velocityX >= 0 || velocityY >= 0) {
                            tileColumn = ((entityX + width) / tileLayer->getTileSize());
                            tileRow = ((entityY + height) / tileLayer->getTileSize());
                            tileid = tiles[tileRow + y][tileColumn + x];
                        } else if(velocityX < 0 || velocityY < 0) {
                            tileColumn = entityX / tileLayer->getTileSize();
                            tileRow = entityY / tileLayer->getTileSize();
                            tileid = tiles[tileRow + y][tileColumn + x];
                        }
                    }
                    
                    if (tileid != 0) {
                        // Reversing velocities is not a good solution for collision resolution
                        entity->getComponent<PhysicsComponent>()->setVelocityX(0);
                        entity->getComponent<PhysicsComponent>()->setVelocityY(0);

                        // TODO: collision resolution
                        // entity->collision();
                    }
                }
            }

            // TODO: colliders will most likely break once we have a camera in engine
            static void checkMapCollision(CollisionLayer* layer) {
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
            
    };
}

#endif // __Collision__