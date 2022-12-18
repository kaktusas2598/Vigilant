#include "StateMachine.hpp"
#include "GameState.hpp"
#include "Engine.hpp"

namespace Vigilant {

    StateMachine::StateMachine(Engine* game) : m_game(game) {}

    StateMachine::~StateMachine() {
        destroy();
    }

    GameState* StateMachine::moveNext() {
        GameState* currentState = getCurrentState();

        if (!currentState->getNextID().empty()) {
            currentStateID = currentState->getNextID();
        }

        return getCurrentState();
    }

    GameState* StateMachine::movePrev() {
        GameState* currentState = getCurrentState();

        if (!currentState->getPrevID().empty()) {
            currentStateID = currentState->getPrevID();
        }

        return getCurrentState();
    }

    void StateMachine::setState(std::string nextStateID) {
        currentStateID = nextStateID;
    }

    void StateMachine::addState(GameState* newScreen) {
        stateMap[newScreen->getID()] = newScreen;
        newScreen->build();
        newScreen->setParentGame(m_game);
    }

    void StateMachine::destroy() {
        //destroy the entire screenList
        for (auto const& s : stateMap) {
            s.second->destroy();
        }

        //no screens left
        //m_states.resize(0);
        currentStateID.erase();
    }

    GameState* StateMachine::getCurrentState() {
        if (currentStateID.empty()) {
            return nullptr;
        }

        return stateMap[currentStateID];
    }
}
