#pragma once

#include <glm/glm.hpp>

#include "Tool.hpp"
#include "../world/Chunk.hpp"
#include "../AppSettings.hpp"

namespace gui {
	
	class Menu {
	
	private:
		Tool _tool;
		world::Chunk *_chunkPtr;
		AppSettings *_settings;
	
		void createAction();
		void deleteAction();
		void digAction();
		void extrudeAction();
		void paintAction();

	public:
		Menu(world::Chunk* chunkPtr, AppSettings *settings);
		~Menu() = default;

		void handleEvent(SDL_Event sdlEvent);

		void drawTools();
		void editCursorPos();
		void colorPicker();
		void lightsSettings();
		void randomGeneration();
		void drawMenu();
		void drawMenuBar();

	};
}