#pragma once


#include <glm/glm.hpp>

namespace gui {
	
	class Menu {
	
	private:
		glm::ivec3 &_cursorPos;

	public:
		Menu();
		~Menu() = default;

		void drawTools();
		void paint();
		void editCursorPos();
		void drawMenu();

		inline glm::ivec3& getCursorPos() const { return _cursorPos; };
		inline void setCursorPos(const glm::ivec3 &cursorPos) { _cursorPos = cursorPos; };
	};
}