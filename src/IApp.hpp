#pragma once

#include "gui/SDLWinWrapper.hpp"

class IApp {
protected:
	// Attributs
	SDLWinWrapper _window;
	bool _running;

	unsigned int _deltaTime;
	unsigned int _lastTime;

	// Methodes
	virtual void handleEvents();
	virtual void handleSDLEvents(SDL_Event sdlEvent) = 0;
	virtual void renderMenu() = 0;

public:
	// Constructor & destructor
	IApp(int width, int height, const char* title);
	virtual ~IApp() = default;

	// Operators & methods
	virtual void beginFrame();
	virtual void endFrame();

	inline void exit() { _running = false; };

	// get & set
	inline SDLWinWrapper& getWinWrapper() { return _window; };
	inline bool isRunning() const { return _running; };

	inline unsigned int getLastTime() const { return _lastTime; };
	inline unsigned int getDeltaTime() const { return _deltaTime; };

};