#ifndef __IENTITY__
#define __IENTITY__

#include <stdio.h>
#include "LoaderParams.hpp"

namespace Vigilant {
	/** \brief IEntity
	*         Abstract Entity Class.
	*
	*  Abstract State Class. Renderable entities must extend from this class
	*  TODO: Future plans: Implement Entity Component System to decouple things like Renderer, Audio and so on
	*/
	class IEntity {

		public:
			virtual void update(float deltaTime) = 0;
			virtual void draw(float deltaTime) = 0;
            virtual void clean() = 0;

			virtual void load(const LoaderParams *params) = 0;

		protected:
            IEntity(){}
			virtual ~IEntity(){}
	};
}

#endif // __IENTITY__