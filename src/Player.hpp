#ifndef __Player__
#define __Player__

#include <string>

#include "SDLEntity.hpp"
#include "EntityFactory.hpp"

namespace Vigilant {
	class Player : public SDLEntity {
		public:
			Player();

			void draw(float deltaTime);
			void update(float deltaTime);
			void clean();
			void load(const LoaderParams *params);
	};

	class PlayerCreator: public BaseCreator {
		public:
			IEntity* createEntity() const {
				return new Player();
			}
	};
}

#endif // __Player__