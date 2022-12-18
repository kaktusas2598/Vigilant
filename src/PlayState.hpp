#ifndef __PlayState__
#define __PlayState__

#include <vector>

#include "IGameState.hpp"
#include "Level.hpp"

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
    };
}

#endif // __PlayState__
