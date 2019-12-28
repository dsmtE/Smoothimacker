#pragma once

#include <glm/glm.hpp>

#include "Tool.hpp"
#include "../world/Chunk.hpp"

namespace gui {
	
	class Menu {
	
	private:
		glm::ivec3* _cursorPos;
		Tool _tool;
		world::Chunk* _chunkPtr;
	
		void createAction();
		void deleteAction();
		void digAction();
		void extrudeAction();
		void paintAction();

	public:
		Menu(glm::ivec3* cursorPointer, world::Chunk* chunkPtr);
		~Menu() = default;

		void handleEvent(SDL_Event sdlEvent);

		void drawTools();
		void editCursorPos();
		void drawMenu();


		//inline void setPointerCursorPos(glm::ivec3 *cursorPos) { cursorPos = _cursorPos; };
	};
}