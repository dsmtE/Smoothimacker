#pragma once

#include <glm/glm.hpp>

#include "Tool.hpp"

namespace gui {
	
	class Menu {
	
	private:
		glm::ivec3* _cursorPos;
		Tool _tool;

	public:
		Menu(glm::ivec3* cursorPointer);
		~Menu() = default;

		void handleEvent(SDL_Event sdlEvent);

		void drawTools();
		void editCursorPos();
		void drawMenu();

		//inline void setPointerCursorPos(glm::ivec3 *cursorPos) { cursorPos = _cursorPos; };
	};
}