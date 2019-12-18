#pragma once

#include "IApp.hpp"
#include "world/Camera.hpp"
#include "world/Cursor.hpp"
#include "openGL/Shader.hpp"
#include "gui/Menu.hpp"

#include <SDL.h>

class App : public IApp {
private:
	// attributs
	world::Camera _cam;
	world::Cursor _cursor;
	openGL::Shader _cursorShader;
	gui::Menu _menu;
	SDL_bool _mouseMode; // handle mouse capture switch
	glm::vec2 _deltaMouse, _deltaWheel;

	// methods
	void handleSDLEvents(SDL_Event sdlEvent) override;
	void handleEvents() override;

public:
	App(int width, int height, const char* title);

	void loop();
	/*
	inline world::Camera& getCam() { return _cam; };
	inline world::Cursor& getCursor() { return _cursor; };
	*/
};