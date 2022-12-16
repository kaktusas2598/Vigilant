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
						// BROAD PHASE
						// Try to dismiss some entities earlier for performance?
						// This dumb check I made surprisingly fixes performance lag caused by calling this method with entities
						// created from Lua and held by EntityManager
						if (entity->transform->getX() > player->transform->getX() + playerSprite->getWidth() * player->transform->getScaleX() ||
							entity->transform->getX() + sprite->getWidth() * entity->transform->getScaleX() < player->transform->getX()) {
							continue;
						}

						// NARROW PHASE
						if(AABB(playerCollider->getCollider(), collider->getCollider())) {
                            // Resolve rigid bodies
                            // TODO: need better algorithm here, also player affects entity, but entity also should affect
                            // player because currently player will just fly through entity,
                            // so maybe add a static method in Physics(RigidBody) component to resolve 2 bodies?
                            auto playerBody = player->getComponent<PhysicsComponent>();
                            auto entityBody = entity->getComponent<PhysicsComponent>();
                            if (entityBody) {
								// 1st try, horrible, player is not affected at all here
								entityBody->applyForceX(playerBody->getAccelerationX() * playerBody->getMass());
								entityBody->applyForceY(playerBody->getAccelerationY() * playerBody->getMass());

								// 2nd try using 1D Newtonian Elastic Collision
                                // m1v11 * m2v21 = m1v12 * m2v22
                                // v12 = (m1 - m2)(m1 + m2)*v11 + 2m2/(m1+m2)*v2
                                // v22 = 2m1/(m1 + m2)*v11 + (m2 - m1)/(m1+m2)*v2
								// Velocity after collision
                                //Vector2D nPVelocity = (pMass - eMass) / (pMass + eMass) * pVelocity + (2 * eMass) / (pMass + eMass) * eVelocity;
                                //Vector2D nEVelocity = (2 * pMass) / (pMass + eMass) * pVelocity + (pMass - eMass) / (pMass + eMass) * eVelocity;
								//playerBody->setVelocity(pNewVelocity);
								//entityBody->setVelocity(eNewVelocity);

                                // 3rd try
								//Vector2D finalVelocity = playerBody->getVelocity() * playerBody->getMass();
								//finalVelocity /= (playerBody->getMass() + entityBody->getMass());
								// Does not do anything at all it seems
								//playerBody->setVelocity(finalVelocity);
								//entityBody->setVelocity(finalVelocity);
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
                int origX, origY;
                if (physics) {
					origX = physics->getPreviousPosition().getX();
					origY = physics->getPreviousPosition().getY();
				} else if (projectile) {
					origX = entity->transform->getX();
					origY = entity->transform->getY();
				} else {
					return;
				}
                // Projectiles: Sprite, Collider, Projectile
                // Actors: Sprite, Collider, Physics
                if ( sprite == 0 || collider == 0) {
                    return;
                }

                int entityX = collider->getCollider().x;
                int entityY = collider->getCollider().y;
                int width = collider->getCollider().w;
                int height = collider->getCollider().h;

                for(std::vector<TileLayer*>::const_iterator it = collisionLayers.begin(); it != collisionLayers.end(); ++it) {
                    TileLayer* tileLayer = (*it);
                    std::vector<std::vector<int>> tiles = tileLayer->getTileIDs();

                    int tileColumn, tileRow, tileid = 0;
                    float velocityX, velocityY;

                    if (physics) {
                        velocityX = physics->getVelocityX();
                        velocityY = physics->getVelocityY();
                    } else if(projectile) {
                        velocityX = projectile->getVelocityX();
                        velocityY = projectile->getVelocityY();
                    } else {
                        return; // If not rigid body nor a projectile
                    }

                    if (entityX > 0 && entityY > 0) {
						// FIXME: collision only works if going down or to the left so if velocity is positive
                        if (velocityX >= 0 || velocityY >= 0) {
                            tileColumn = ((entityX + width) / (tileLayer->getTileSize() * tileLayer->getScale()));
                            tileRow = ((entityY + height) / (tileLayer->getTileSize() * tileLayer->getScale()));
							if (tileRow < tileLayer->getMapHeight() && tileColumn < tileLayer->getMapWidth())
                                tileid = tiles[tileRow][tileColumn];
                        } else if(velocityX < 0 || velocityY < 0) {
                            tileColumn = entityX / (tileLayer->getTileSize() * tileLayer->getScale());
                            tileRow = entityY / (tileLayer->getTileSize() * tileLayer->getScale());
							if (tileRow < tileLayer->getMapHeight() && tileColumn < tileLayer->getMapWidth())
                                tileid = tiles[tileRow][tileColumn];
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
                        } else if (projectile) {
                            entity->destroy(); // Destroy projectiles so that they don't go through walls
                        }
                    }
                }
            }

            // TODO: colliders will most likely break once we have a camera in engine
            static void checkMapCollision(CollisionLayer* layer) {
                //std::vector<SDL_Rect> colliders = layer->getColliders();

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
