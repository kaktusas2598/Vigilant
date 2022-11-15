#ifndef __PlayState__
#define __PlayState__

#include <vector>

#include "IGameState.hpp"
#include "SDLEntity.hpp"
#include "Level.hpp"
// class IEntity;
// class SDLEntity;

namespace Vigilant {
    class PlayState : public IGameState {
    public:
        int getNextStateIndex() const { return 0; }
		int getPrevStateIndex() const { return 0; }
        
        void build() {}
		void destroy() {}
        void onEntry();
        void onExit();
        
        void update(float deltaTime);
        void draw(float deltaTime);
        
        std::string getStateID() const { return playID; }
        
    private:
        Level* level;

        static const std::string playID;
        std::vector<IEntity*> gameEntities;
        
        bool checkCollision(SDLEntity* p1, SDLEntity* p2);
    };
}

#endif // __PlayState__