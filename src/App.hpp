#pragma once

#include "IApp.hpp"

#include "world/Camera.hpp"
#include "world/Cursor.hpp"
#include "openGL/Shader.hpp"
#include "world/Chunk.hpp"
#include "world/ControlPts.hpp"
#include "gui/Menu.hpp"
#include <SDL.h>

#include "AppSettings.hpp"

class App : public IApp {
private:
	// attributs
	world::Camera _cam;
	openGL::Shader _cursorShader, _chunkShader, _controlPtsShader;

	world::Cursor _cursor;
	world::Chunk _chunk;
	world::ControlPts _controlPts;
	gui::Menu _menu;

	AppSettings _settings;

	// methods
	void handleSDLEvents(SDL_Event sdlEvent) override;
	void handleEvents() override;

public:
	App(int width, int height, const char* title);

	void loop();
};