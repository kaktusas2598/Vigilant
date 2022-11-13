#pragma once

#include <stdio.h>
#include "LoaderParams.hpp"

namespace Vigilant
{
	// class IEngine;


	/** \brief IEntity
	*         Abstract Entity Class.
	*
	*  Abstract State Class. Renderable entities must extend from this class, this is temporary solution!
	*/
	class IEntity {

		public:
			virtual void update(float deltaTime) = 0;
			virtual void draw(float deltaTime) = 0;
            virtual void clean() = 0;

			virtual void load(const LoaderParams *params) = 0;
			/// Sets m_game to be the parent game
			// void setParentGame(IEngine* game) { m_game = game; }

		protected:
            IEntity(){}
			virtual ~IEntity(){}

			// IEngine* m_game = nullptr; ///< Pointer to Engine
	};
}
