#ifndef __GameState__
#define __GameState__

#include <stdio.h>
#include <string>
#include <vector>
#include "Level.hpp"

namespace Vigilant
{
    class Engine;

    /**
     * Enumeration of available states per GameState
     */
    enum class ScreenState {
        NONE,
        RUNNING, //< state is running
        EXIT_APPLICATION, //< exit application
        CHANGE_NEXT, ///< change to next screen
        CHANGE_PREVIOUS ///< change to previous screen
    };


    /** \brief GameState
     *
     *  State Class. Assets loaded from XML and switched in Lua
     *  @sa StateMachine
     */
    class GameState {

        public:
            friend class StateMachine;

            GameState(const std::string& id) {
                stateID = id;
                level = nullptr;
            }
            ~GameState(){ }

            std::string getID() const { return stateID; }
            std::string getNextID() const { return nextStateID; }
            void setNextID(const std::string& id) { nextStateID = id; }
            std::string getPrevID() const { return prevStateID; }
            void setPrevID(const std::string& id) { prevStateID = id; }

            void build() {}
            void destroy() {}

            void onEntry();
            void onExit();

            /// Called in game loop
            void update(float deltaTime);
            void draw(float deltaTime) {
                if (level != nullptr)
                    level->render();
            }

            /// Sets m_game to be the parent game
            void setParentGame(Engine* game) { m_game = game; }
            void setRunning() {
                currentScreenState = ScreenState::RUNNING;
            }

            ScreenState getScreenState() { return currentScreenState; } ///< current ScreenState enum getter
            void setScreenState(ScreenState screenState) { currentScreenState = screenState; }

        protected:
            // State holds it's own ID and ID for the next state which can be changed
            std::string stateID;
            std::string nextStateID;
            std::string prevStateID;

            std::vector<std::string> textureIDs;
            std::vector<std::string> soundIDs;

            Level* level;

            ScreenState currentScreenState = ScreenState::NONE;
            Engine* m_game = nullptr; ///< Pointer to Engine
    };
}

#endif // __GameState__