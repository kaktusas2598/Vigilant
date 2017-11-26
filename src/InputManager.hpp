#pragma once

#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <stdio.h>
#include <unordered_map>

#include <iostream>

namespace Vigilant
{

	/*! \brief InputManager
	 *         Keyboard and Mouse input manager.
	 *
	 *  Manages unordered map currently and previously pressed keys and also holds mouse coordinates.
	 *  Can be used to set pressed keys and mouse coords, and also to query for presse keys or coordinates.
	 *  @sa IEngine::handleEvents
	 */
	class InputManager
	{
		public:
			InputManager();
			~InputManager();

			/// Copies current key map to previous one, gets called in Main Engine loop
			void update();

			/// Set the key value in map to true(pressed)
			void pressKey(unsigned int keyID);
			/// Set the key value in map to false(released)
			void releaseKey(unsigned int keyID);

			/// Helper func for updating the current event type
			void setEventType(Uint32 eventType) { m_eventType = eventType; }

			/// Tells the input manager the location of the cursor
			void setMouseCoords(float x, float y);

			/// Returns true if key is held down
			bool isKeyDown(unsigned int keyID);

			/// Returns true if key is pressed
			bool isKeyPressed(unsigned int keyID);

			/// Updates the mouse wheel
			void setMouseWheel(int m);
			int mousewheel = 0; ///< Mouse wheel's value

			/// Add characters into inChars
			void addInputCharacter(char c);
			/// Add characters into inChars from a string
			void addInputCharacters(const char* chars);

			glm::vec2 getMouseCoords() const { return m_mouseCoords; } ///<Mouse coords's geeter
			Uint32 getEventType() { return m_eventType; } //< SDL_Event type's getter
			const char* getInChars() { return m_inChars; }

		private:

			/// Identical to isKeyDown but searches the previous key map
			bool wasKeyDown(unsigned int keyID);

			std::unordered_map<unsigned int, bool> m_keyMap; ///< Holds map of SDL key values versus pressed/released bool values
			std::unordered_map<unsigned int, bool> m_prevKeyMap; ///< Holds map of previously pressed keys

			glm::vec2 m_mouseCoords; ///< GLM vector2 holding mouse coordinates

			Uint32 m_eventType; ///< event type: keypress, mousemotion, etc...

			char m_inChars[1];
	};
}
