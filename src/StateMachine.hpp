#ifndef __StateMachine__
#define __StateMachine__

#define STATE_NONE -1

#include <stdio.h>
#include <vector>
#include <string>
#include <map>

namespace Vigilant {
    class Engine;
    class GameState;

    /*! \brief StateMachine
     *         Finite State Machine Manager class.
     *
     * Finite State Machine. Manages std::vector of GameState instances
     * Limitations: changes between states sequentually
     *  @sa GameState, Engine
     */
    class StateMachine {
        public:
            StateMachine(Engine* game);
            ~StateMachine();

            GameState* moveNext();
            GameState* movePrev();

            void setState(std::string nextStateID);
            void addState(GameState* newState);

            void destroy();

            GameState* getCurrentState();

        protected:
            std::map<std::string, GameState*> stateMap; ///< State collection mapped against state name
            std::string currentStateID; ///< Indicates urrent state in states collection, -1 indicates none
            Engine* m_game = nullptr; ///< Pointer to main engine's class
    };
}


#endif // __StateMachine__
