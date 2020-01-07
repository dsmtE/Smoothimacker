#pragma once

#include <glm/glm.hpp>

#include "Tool.hpp"
#include "../world/Chunk.hpp"
#include "../AppSettings.hpp"

namespace gui {
	///\brief menu class that takes care of drawing the user interface
	class Menu {
	
	private:
		Tool _tool;
		world::Chunk *_chunkPtr;
		AppSettings *_settings;

		float _rbfDysplayRange;
		int _selectedRbfId;
	
		void createAction();
		void deleteAction();
		void digAction();
		void extrudeAction();
		void paintAction();

	public:
		///\brief Construct a new Menu
		///
		///\param chunkPtr pointer to our chunk
		///\param settings pointer to setting used to have acces to app attributs
		Menu(world::Chunk* chunkPtr, AppSettings *settings);

		///\brief Destroy the Menu object
		~Menu() = default;

		///\brief delegated function that manages sdl events
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