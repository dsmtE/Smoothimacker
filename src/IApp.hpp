#pragma once

#include "openGL/SDLWinWrapper.hpp"

class IApp {
protected:
	// Attributs
	openGL::SDLWinWrapper _window;

	unsigned int _deltaTime;
	unsigned int _lastTime;

	// Methodes
	virtual void handleEvents();
	virtual void handleSDLEvents(SDL_Event sdlEvent) = 0;

public:
	// Constructor & destructor
	IApp(int width, int height, const char* title);
	virtual ~IApp() = default;

	// Operators & methods
	virtual void beginFrame();
	virtual void endFrame();

	inline void exit() { _window.exit(); };

	// get & set
	inline openGL::SDLWinWrapper& getWinWrapper() { return _window; };
	inline bool isRunning() const { return _window.isRunning(); };

	inline unsigned int getLastTime() const { return _lastTime; };
	inline unsigned int getDeltaTime() const { return _deltaTime; };
	
	glm::ivec2 windowSize() const;

};