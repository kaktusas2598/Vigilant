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
#include "InputComponent.hpp"
#include "ScriptEngine.hpp"

#include <iostream>

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

			//static bool checkEntityOnEntityCollision(Entity* actor, const std::vector<std::shared_ptr<Entity>>& entities) {
				//return false;
			//}

            static void checkProjectileEntityCollision(Entity* projectile, const std::vector<Entity*> entities) {
            	// TODO: implement
            	// Sane as playerEntityCollision but need to call event not for entity, but ENtity* shooter
            	// Or another event?
				// This is tempporary because lua wont know if player collided with entity or players projectile
				// Also npcs will not be able to create projectiles themselves
				//Entity* owner = projectile->getComponent<ProjectileComponent>()->getShooter();
				//if (checkEntityOnEntityCollision(projectile, entities)) {
					//owner->getComponent<ScriptComponent>();
				//}
			}
            //static void checkPlayerEntityCollision(Entity* player, const std::vector<Entity*> entities) {
				//if (checkEntityOnEntityCollision(player, entities)) {
				//}
			//}

            static void checkPlayerEntityCollision(Entity* player, const std::vector<Entity*>& entities) {
				auto playerCollider = player->getComponent<ColliderComponent>();
				auto playerSprite = player->getComponent<SpriteComponent>();
				float playerX = player->transform->getX();
				float playerY = player->transform->getY();
				// Calculate real entity position based on collider box
				// This will only work fine if sprites are perfectly centered in each frame
				float colliderOffsetX = (playerSprite->getWidth() - playerCollider->getCollider().w)/2 * player->transform->getScaleX();
				float colliderOffsetY = (playerSprite->getHeight() - playerCollider->getCollider().h)/2 * player->transform->getScaleY();
				//TODO: improve
				playerX += colliderOffsetX;
				playerX += colliderOffsetY;

				int width = playerCollider->getCollider().w * player->transform->getScaleX();
				int height = playerCollider->getCollider().h * player->transform->getScaleY();

                // FIXME: SDL_Rect holds ints so this is going to be not good..
                SDL_Rect playerColliderRect{(int)playerX, (int)playerY, width, height};

                for (auto &entity : entities) {
					auto collider = entity->getComponent<ColliderComponent>();
					auto sprite = entity->getComponent<SpriteComponent>();
					auto isPlayer = entity->getComponent<InputComponent>();
					auto projectile = entity->getComponent<ProjectileComponent>();
					// For now player will not colide wiht projectiles
					if (isPlayer || projectile) {
						continue;
					}
					if (collider && sprite) {
						// Try to dismiss some entities earlier for performance?
						// This dumb check I made surprisingly fixes performance lag caused by calling this method with entities
						// created from Lua and held by EntityManager
						if (entity->transform->getX() > player->transform->getX() + playerSprite->getWidth() * player->transform->getScaleX() ||
							entity->transform->getX() + sprite->getWidth() * entity->transform->getScaleX() < player->transform->getX()) {
							continue;
						}

						// Calculate real entity position based on collider box
						// This will only work fine if sprites are perfectly centered in each frame
						//std::cout << "Valid entity found for player collision check" << std::endl;
						colliderOffsetX = (sprite->getWidth() - collider->getCollider().w)/2 * entity->transform->getScaleX();
						colliderOffsetY = (sprite->getHeight() - collider->getCollider().h)/2 * entity->transform->getScaleY();
						//TODO: improve
						float entityX = entity->transform->getX() + colliderOffsetX;
						float entityY = entity->transform->getY() + colliderOffsetY;

						SDL_Rect entityColliderRect{
							(int)entityX,
							(int)entityY,
							(int)(collider->getCollider().w * entity->transform->getScaleX()),
							(int)(collider->getCollider().h * entity->transform->getScaleY())
						};

						if(AABB(playerColliderRect, entityColliderRect)) {
                            // Resolve rigid bodies
                            auto playerBody = player->getComponent<PhysicsComponent>();
                            auto entityBody = entity->getComponent<PhysicsComponent>();
                            if (entityBody) {
                                entityBody->applyForceX(playerBody->getAccelerationX() * playerBody->getMass());
                                entityBody->applyForceY(playerBody->getAccelerationY() * playerBody->getMass());
                            }

							// std::cout << "Player and entity collision detected" << std::endl;
							std::string listener = playerCollider->getListener();
							ScriptEngine::Instance()->onCollide(listener, player->id->get(), entity->id->get());
						}
					}

                }
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

                    // This is the reason why enabling Solids using IMGui broke collision algorithm:
                    // tile layer position is used for rendering and once we enabled rendering for collision layers
                    // it started affecting the actual position and this is why collision broke, fixed!
                    // TODO: now we can simplify this a lot, position is probably not needed here at all
                    //Vector2D layerPos = tileLayer->getPosition();
                    Vector2D layerPos{0, 0};
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
