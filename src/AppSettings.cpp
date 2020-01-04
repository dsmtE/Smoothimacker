#include "AppSettings.hpp"

// ---------- AppSettings ----------//

// init static const class setting
const float AppSettings::camMaxSpeed = 30.f;
const float AppSettings::camMinSpeed = 5.f;

AppSettings::AppSettings(glm::ivec3* cursorPos, float* cameraSpeed, openGL::SDLWinWrapper* winPtr) :
	_cursorPos(cursorPos),
	_cameraSpeed(cameraSpeed),
	_winPtr(winPtr),
	_rayCastingEnable(false),
	_relativeMouse(SDL_FALSE),
	_sunDir(1.0f, -1.0f, 1.0f),
	_sunColor(1.0f, 1.0f, 1.0f) {
}

glm::ivec2 AppSettings::windowSize() const {
	glm::ivec2 size;
	SDL_GetWindowSize(_winPtr->getWindow(), &(size.x), &(size.y));
	return size;
}