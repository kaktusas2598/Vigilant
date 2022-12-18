#ifndef __ScriptEngine__
#define __ScriptEngine__

#include "LuaScript.hpp"

#include "Entity.hpp"

#include "SpriteComponent.hpp"
#include "PhysicsComponent.hpp"
#include "ProjectileComponent.hpp"
#include "ColliderComponent.hpp"
#include "InputComponent.hpp"
#include "ButtonComponent.hpp"
#include "UILabelComponent.hpp"

#include "SoundManager.hpp"
#include "TextureManager.hpp"
#include "Engine.hpp"
#include "IGameState.hpp"
#include "ParticleSystem.hpp"
#include "EntityManager.hpp"

#include <string>

namespace Vigilant {

    // TODO:
    // Must support multiple scripts, global script for common functionality, config scripts
    // Might work similar to TextureManager, maybe recall to ScriptManager?
    // addScript(ScriptType)
    enum ScriptType {
        SCRIPT_CONFIG = 0,
        SCRIPT_GLOBALS = 1,
        SCRIPT_ENTITIES = 2
    };

    class ScriptEngine {
        public:
            ScriptEngine();
            ~ScriptEngine() {
            }

            static ScriptEngine* Instance() {
                if(s_pInstance == 0) {
                    s_pInstance = new ScriptEngine();
                    return s_pInstance;
                }
                return s_pInstance;
            }

            // Scripts will probably not run every frame so this will be empty method
            // Maybe this could be used to limit insane amount of times onCollide() event is triggered?
            //
            void update(float deltaTime);

            // Setup metatables and bind c++ methods, load script
            // Tried loading init script for all states, functions can be shared, but variables
            // don't seem to be shared even in the same lua_state?
            void init(std::string fileName);

            // Loads and runs script
            void loadScript(std::string fileName);
            void loadChunk(const char* chunk);

            // Close all scripts
            void close() {
                script->close();
            }

            // Listeners
            void onInput(std::string listener, int thisId, unsigned int keyID);
            void onCollide(std::string listener, int thisId, int secondId);
            // void onCollide(Entity* entity);

            // Lua API is written in C so only static C++ class methods can be wrapped for lua
            // Alternative way would be to use global functions
            //------------------------------------------------------------------------------------
            static int lua_playSound(lua_State *L) {
                std::string soundID = (std::string)lua_tostring(L, 1);
                TheSoundManager::Instance()->playSound(soundID, 0);
                return 0;
            }

            static int lua_playMusic(lua_State *L) {
                std::string soundID = (std::string)lua_tostring(L, 1);
                TheSoundManager::Instance()->playMusic(soundID, 0);
                return 0;
            }

            static int lua_addParticleEmitter(lua_State *L) {
                float originX = (float)lua_tonumber(L, 1);
                float originY = (float)lua_tonumber(L, 2);
                std::string type = (std::string)lua_tostring(L, 3);
                ParticleSystem::Instance()->addEmitter(Vector2D{originX, originY}, type);
                return 0;
            }


            static int lua_getMapWidth(lua_State *L) {
                int mapWidth = TheEngine::Instance()->getMapWidth();
                lua_pushnumber(L, mapWidth);
                return 1;
            }

            static int lua_getMapHeight(lua_State *L) {
                int mapHeight = TheEngine::Instance()->getMapHeight();
                lua_pushnumber(L, mapHeight);
                return 1;
            }

            static int lua_getScreenWidth(lua_State *L) {
                int screenWidth = TheEngine::Instance()->getScreenWidth();
                lua_pushnumber(L, screenWidth);
                return 1;
            }

            static int lua_getScreenHeight(lua_State *L) {
                int screenHeight = TheEngine::Instance()->getScreenHeight();
                lua_pushnumber(L, screenHeight);
                return 1;
            }

            // TODO: this is temporary, it will need to accept state id as param
            static int lua_changeState(lua_State *L) {
                TheEngine::Instance()->getStateMachine()->getCurrentState()->setScreenState(ScreenState::CHANGE_NEXT);
                return 1;
            }

            static int lua_quit(lua_State *L) {
                TheEngine::Instance()->setRunning(false);
                return 0;
            }


            // Allocates new user data and sends reference to EntityManager
            static int lua_createEntity(lua_State *L);
            // Lua __gc and removes reference from EntityManager
            static int lua_destroyEntity(lua_State *L);

            // Manual remove from entity manager array
            static int lua_removeEntity(lua_State *L) {
                Entity* entity = (Entity*)lua_touserdata(L, -1);
                EntityManager::Instance()->remove(entity->id->get());
                return 0;
            }

            static int lua_entityId(lua_State *L) {
                Entity* entity = (Entity*)lua_touserdata(L, -1);
                lua_pushnumber(L, entity->id->get());
                return 1;
            }

            static int lua_getTag(lua_State *L) {
                Entity* entity = (Entity*)lua_touserdata(L, -1);
                lua_pushstring(L, entity->id->getTag().c_str());
                return 1;
            }

            static int lua_setTag(lua_State *L) {
                Entity* entity = (Entity*)lua_touserdata(L, 1);
                std::string tag = (std::string)lua_tostring(L, 2);
                entity->id->setTag(tag);
                return 0;
            }

            static int lua_getType(lua_State *L) {
                Entity* entity = (Entity*)lua_touserdata(L, -1);
                lua_pushstring(L, entity->id->getType().c_str());
                return 1;
            }

            static int lua_setType(lua_State *L) {
                Entity* entity = (Entity*)lua_touserdata(L, 1);
                std::string type = (std::string)lua_tostring(L, 2);
                entity->id->setType(type);
                return 0;
            }

            static int lua_getX(lua_State *L) {
                Entity* entity = (Entity*)lua_touserdata(L, -1);
                lua_pushnumber(L, entity->transform->getX());
                return 1;
            }

            static int lua_getY(lua_State *L) {
                Entity* entity = (Entity*)lua_touserdata(L, -1);
                lua_pushnumber(L, entity->transform->getY());
                return 1;
            }

            static int lua_teleportEntity(lua_State *L) {
                Entity* entity = (Entity*)lua_touserdata(L, -3);
                float x = (float)lua_tonumber(L, -2);
                float y = (float)lua_tonumber(L, -1);

                entity->transform->setX(x);
                entity->transform->setY(y);
                return 0;
            }

            static int lua_scaleEntity(lua_State *L) {
                Entity* entity = (Entity*)lua_touserdata(L, -3);
                float scaleX = (float)lua_tonumber(L, -2);
                float scaleY = (float)lua_tonumber(L, -1);

                entity->transform->setScaleX(scaleX);
                entity->transform->setScaleY(scaleY);
                return 0;
            }

            static int lua_addSprite(lua_State *L) {
                Entity* entity = (Entity*)lua_touserdata(L, 1);
                std::string id = (std::string)lua_tostring(L, 2);
                std::string fileName = (std::string)lua_tostring(L, 3);
                float width = (int)lua_tonumber(L, 4);
                float height = (int)lua_tonumber(L, 5);

                TheTextureManager::Instance()->load(fileName, id);
                entity->addComponent<SpriteComponent>();
                entity->getComponent<SpriteComponent>()->load(id, width, height);
                return 0;
            }

            static int lua_setAbsolutePosition(lua_State *L) {
                Entity* entity = (Entity*)lua_touserdata(L, 1);
                bool absol = (bool)lua_toboolean(L, 2);
                entity->getComponent<SpriteComponent>()->setAbsolute(absol);
                return 0;
            }


            static int lua_addAnimation(lua_State *L) {
                Entity* entity = (Entity*)lua_touserdata(L, 1);
                std::string name = (std::string)lua_tostring(L, 2);
                int row = (int)lua_tonumber(L, 3);
                int numFrames = (int)lua_tonumber(L, 4);
                Animation anim = {row, numFrames};
                entity->getComponent<SpriteComponent>()->addAnimation(name, anim);
                return 0;
            }

            // TODO: rename here and in Lua to playAnimation()
            static int lua_setAnimation(lua_State *L) {
                Entity* entity = (Entity*)lua_touserdata(L, 1);
                std::string name = (std::string)lua_tostring(L, 2);
                // Testing optional param
                // I don't know if this is good practise but if I dont provide third paradm, this casts
                // to false seemingly always, so I can use optional 3rd param to set oneShot animation
                bool oneShot = (bool)lua_toboolean(L, 3);

                entity->getComponent<SpriteComponent>()->playAnimation(name, oneShot);
                if (oneShot) {
                }

                return 0;
            }

            static int lua_addInput(lua_State *L) {
                Entity* entity = (Entity*)lua_touserdata(L, 1);
                entity->addComponent<InputComponent>();
                return 0;
            }

            static int lua_setInputListener(lua_State *L) {
                Entity* entity = (Entity*)lua_touserdata(L, 1);
                std::string listener = (std::string)lua_tostring(L, 2);
                entity->getComponent<InputComponent>()->setListener(listener);
                return 0;
            }

            static int lua_addButton(lua_State *L) {
                Entity* entity = (Entity*)lua_touserdata(L, 1);
                entity->addComponent<ButtonComponent>();
                return 0;
            }

            static int lua_setButtonListener(lua_State *L) {
                Entity* entity = (Entity*)lua_touserdata(L, 1);
                std::string listener = (std::string)lua_tostring(L, 2);
                entity->getComponent<ButtonComponent>()->setListener(listener);
                return 0;
            }

            static int lua_addLabel(lua_State *L) {
                Entity* entity = (Entity*)lua_touserdata(L, 1);
                entity->addComponent<UILabelComponent>();
                int x = (int)lua_tonumber(L, 2);
                int y = (int)lua_tonumber(L, 3);
                std::string text = (std::string)lua_tostring(L, 4);
                std::string fontId = (std::string)lua_tostring(L, 5);
                int r = (int)lua_tonumber(L, 6);
                int g = (int)lua_tonumber(L, 7);
                int b = (int)lua_tonumber(L, 8);
                int a = (int)lua_tonumber(L, 9);
                SDL_Color color{(Uint8)r, (Uint8)g, (Uint8)b, (Uint8)a};
                entity->getComponent<UILabelComponent>()->load(x, y, text, fontId, color);
                return 0;
            }

            static int lua_setLabel(lua_State *L) {
                Entity* entity = (Entity*)lua_touserdata(L, 1);
                std::string text = (std::string)lua_tostring(L, 2);
                entity->getComponent<UILabelComponent>()->setValue(text);
                return 0;
            }

            static int lua_setLabelAlignment(lua_State *L) {
                Entity* entity = (Entity*)lua_touserdata(L, 1);
                int alignH = (int)lua_tonumber(L, 2);
                int alignV = (int)lua_tonumber(L, 3);
                entity->getComponent<UILabelComponent>()->setAlignment(static_cast<Align>(alignH), static_cast<VerticalAlign>(alignV));
                return 0;
            }

            static int lua_addCollider(lua_State *L) {
                Entity* entity = (Entity*)lua_touserdata(L, 1);
                int width = (int)lua_tonumber(L, 2);
                int height = (int)lua_tonumber(L, 3);
                entity->addComponent<ColliderComponent>()->load(width, height);
                return 0;
            }

            static int lua_setCollideListener(lua_State *L) {
                Entity* entity = (Entity*)lua_touserdata(L, 1);
                std::string listener = (std::string)lua_tostring(L, 2);
                entity->getComponent<ColliderComponent>()->setListener(listener);
                return 0;
            }

            static int lua_addPhysics(lua_State *L) {
                Entity* entity = (Entity*)lua_touserdata(L, 1);
                float mass = (int)lua_tonumber(L, 2);
                float frictionCoef = (int)lua_tonumber(L, 3);
                entity->addComponent<PhysicsComponent>();
                entity->getComponent<PhysicsComponent>()->setMass(mass);
                entity->getComponent<PhysicsComponent>()->setFrictionCoefficient(frictionCoef);
                // entity->getComponent<PhysicsComponent>()->load(float velX, float velY, float accX, float accY, float gX, float gY)
                return 0;
            }

            static int lua_applyForceX(lua_State *L) {
                Entity* entity = (Entity*)lua_touserdata(L, 1);
                float force = (float)lua_tonumber(L, 2);
                entity->getComponent<PhysicsComponent>()->applyForceX(force);
                return 0;
            }

            static int lua_applyForceY(lua_State *L) {
                Entity* entity = (Entity*)lua_touserdata(L, 1);
                float force = (float)lua_tonumber(L, 2);
                entity->getComponent<PhysicsComponent>()->applyForceY(force);
                return 0;
            }

            static int lua_addProjectile(lua_State *L) {
                Entity* entity = (Entity*)lua_touserdata(L, 1);
                Entity* shooter = (Entity*)lua_touserdata(L, 2);
                int speed = (int)lua_tonumber(L, 3);
                int range = (int)lua_tonumber(L, 4);
                Vector2D velocity = shooter->getComponent<PhysicsComponent>()->getVelocity();
                velocity.normalize();
                velocity *= speed;
                entity->addComponent<ProjectileComponent>()->load(shooter, range, speed, velocity);
                return 0;
            }

            // Temporary place, this method should go to dedicated scripting classes
            bool luaOk(lua_State* L, int call) {
                if (call != LUA_OK) {
                    std::string errorMessage = lua_tostring(L, -1);
                    std::cout << errorMessage << std::endl;
                    Logger::Instance()->error(errorMessage.c_str());
                    return false;
                }
                return true;
            }
            //------------------------------------------------------------------------------------

        private:
            static ScriptEngine* s_pInstance;

            lua_State* state;
            // Used for init script and other scripts will load onto same state
            LuaScript* script;
            std::string fileName;
            // This identifies which table to get from lua ascript, dont like this for some reason
            std::string id;
    };
}

#endif // __ScriptEngine__
