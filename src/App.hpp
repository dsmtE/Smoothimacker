#pragma once

#include "IApp.hpp"

#include "world/Camera.hpp"
#include "world/Cursor.hpp"
#include "openGL/Shader.hpp"
#include "world/Chunk.hpp"
#include "gui/Menu.hpp"
#include <SDL.h>

#include "AppSettings.hpp"

class App : public IApp {
private:
	// attributs
	world::Camera _cam;
	world::Cursor _cursor;
	openGL::Shader _cursorShader;
	openGL::Shader _chunkShader;
	world::Chunk _chunk;
	gui::Menu _menu;

	AppSettings _settings;

	// methods
	void handleSDLEvents(SDL_Event sdlEvent) override;
	void handleEvents() override;

public:
	App(int width, int height, const char* title);

	void loop();
};