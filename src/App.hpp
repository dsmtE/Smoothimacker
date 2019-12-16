#include pragme once

#include "IApp.hpp"
#include "world/Camera.hpp"

#include <SDL.h>

class App : public IApp {
private:
	// attributs
	world::Camera _cam;
	SDL_bool _mouseMode; // handle mouse capture switch
	glm::vec2 _deltaMouse, _deltaWheel;

	// methods
	void handleSDLEvents(SDL_Event sdlEvent) override;
	void handleEvents() override;
	void renderMenu() override;

public:

	App(int width, int height, const char* title, const world::Camera &cam);
	inline world::Camera& getCam() { return _cam; };
};