#ifndef __Entity__
#define __Entity__

#include <stdio.h>
#include "LoaderParams.hpp"

namespace Vigilant {
	/** \brief Entity
	*         Abstract Entity Class.
	*
	*  Abstract State Class. Renderable entities must extend from this class
	*  TODO: Future plans: Implement Entity Component System to decouple things like Renderer, Audio and so on
	*/
	class Entity {

		public:
			virtual void update(float deltaTime) = 0;
			virtual void draw(float deltaTime) = 0;
            virtual void clean() = 0;

			virtual void load(const LoaderParams *params) = 0;

		protected:
            Entity(){}
			virtual ~Entity(){}
	};
}

#endif // __Entity__