#ifndef __Collision__
#define __Collision__

#include <string>

#include "CollisionLayer.hpp"

#include "TileLayer.hpp"
#include "Vector2D.hpp"

#include "PhysicsComponent.hpp"
#include "SpriteComponent.hpp"
#include "ColliderComponent.hpp"
#include "ProjectileComponent.hpp"

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

            static void checkPlayerEntityCollision(Entity* player, const std::vector<Entity*> entities) {
                // TODO: implement
            }

            static void checkMapCollision(Entity* entity, const std::vector<TileLayer*>& collisionLayers) {
                // Check if entity has required components
                // TODO sprite should not be needed
                auto physics = entity->getComponent<PhysicsComponent>();
                auto sprite = entity->getComponent<SpriteComponent>();
                auto collider = entity->getComponent<ColliderComponent>();
                auto projectile = entity->getComponent<ProjectileComponent>();
                int origX = entity->transform->getX();
                int origY = entity->transform->getY();
                // Projectiles: Sprite, Collider, Projectile
                // Actors: Sprite, Collider, Physics
                if ( sprite == 0 || collider == 0) {
                    return;
                }

                for(std::vector<TileLayer*>::const_iterator it = collisionLayers.begin(); it != collisionLayers.end(); ++it) {
                    TileLayer* tileLayer = (*it);
                    std::vector<std::vector<int>> tiles = tileLayer->getTileIDs();

                    Vector2D layerPos = tileLayer->getPosition();
                    int x, y, tileColumn, tileRow, tileid = 0;
                    float velocityX, velocityY;

                    x = layerPos.getX() / (tileLayer->getTileSize() * tileLayer->getScale());
                    y = layerPos.getY() / (tileLayer->getTileSize() * tileLayer->getScale());

                    if (physics) {
                        velocityX = physics->getVelocityX();
                        velocityY = physics->getVelocityY();
                    } else if(projectile) {
                        velocityX = projectile->getVelocityX();
                        velocityY = projectile->getVelocityY();
                    } else {
                        return; // If not rigid body nor a projectile
                    }
                    float entityX = entity->transform->getX();
                    float entityY = entity->transform->getY();

                    // Calculate real entity position based on collider box
                    // This will only work fine if sprites are perfectly centered in each frame
                    float colliderOffsetX = (sprite->getWidth() - collider->getCollider().w)/2 * entity->transform->getScaleX();
                    float colliderOffsetY = (sprite->getHeight() - collider->getCollider().h)/2 * entity->transform->getScaleY();
                    if (velocityX > 0)
                    	entityX += colliderOffsetX;
					else
						entityX -= colliderOffsetX;
					if (velocityY > 0)
                    	entityY += colliderOffsetY;
					else
						entityY -= colliderOffsetY;

                    int width = collider->getCollider().w * entity->transform->getScaleX();
                    int height = collider->getCollider().h * entity->transform->getScaleY();

                    if (entityX > 0 && entityY > 0) {
                        if (velocityX >= 0 || velocityY >= 0) {
                            tileColumn = ((entityX + width) / (tileLayer->getTileSize() * tileLayer->getScale()));
                            tileRow = ((entityY + height) / (tileLayer->getTileSize() * tileLayer->getScale()));
                            if (tileRow + y < tileLayer->getMapHeight() && tileColumn + x < tileLayer->getMapWidth())
                                tileid = tiles[tileRow + y][tileColumn + x];
                        } else if(velocityX < 0 || velocityY < 0) {
                            tileColumn = entityX / (tileLayer->getTileSize() * tileLayer->getScale());
                            tileRow = entityY / (tileLayer->getTileSize() * tileLayer->getScale());
                            if (tileRow + x < tileLayer->getMapHeight() && tileColumn + y < tileLayer->getMapWidth())
                                tileid = tiles[tileRow + y][tileColumn + x];
                        }
                    }

                    // Collision between entity and a tile detected!
                    if (tileid != 0) {
                        entity->transform->setX(origX);
                        entity->transform->setY(origY);
                        // Reversing velocities is not a good solution for collision resolution
                        if (physics) {
						    entity->getComponent<PhysicsComponent>()->setVelocityX(0);
                            entity->getComponent<PhysicsComponent>()->setVelocityY(0);
                        }else if (projectile) {
                            entity->destroy(); // Destroy projectiles so that they don't go through walls
                        }

                        // TODO: collision resolution
                        // Needs to be some kind of event which can be then sent to Lua
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
