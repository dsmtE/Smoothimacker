#pragma once


#include <glm/glm.hpp>

namespace gui {
	
	class Menu {
	
	private:
		glm::ivec3* _cursorPos;

	public:
		Menu(glm::ivec3* cursorPointer);
		~Menu() = default;

		void drawTools();
		void paint();
		void editCursorPos();
		void drawMenu();

		//inline void setPointerCursorPos(glm::ivec3 *cursorPos) { cursorPos = _cursorPos; };
	};
}