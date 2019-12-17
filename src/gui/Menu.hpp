#pragma once


#include <glm/glm.hpp>

namespace gui {
	
	class Menu {
	
	private:
		glm::ivec3 &_cursorPos;

	public:
		Menu();
		~Menu() = default;

		void drawMenu();
		inline void setCursorPos(const glm::ivec3 &cursorPos) { _cursorPos = cursorPos; };
	};
}