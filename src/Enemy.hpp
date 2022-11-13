#ifndef __Enemy__
#define __Enemy__

#include <string>

#include "SDLEntity.hpp"
#include "EntityFactory.hpp"

namespace Vigilant {
	class Enemy : public SDLEntity {
		public:
			Enemy();

			void draw(float deltaTime);
			void update(float deltaTime);
			void clean();
			void load(const LoaderParams *params);
	};

	class EnemyCreator: public BaseCreator {
		public:
			IEntity* createEntity() const {
				return new Enemy();
			}
	};
}

#endif // __Player__