#include "AppSettings.hpp"

// ---------- AppSettings ----------//

// init static const class setting
const float AppSettings::camMaxSpeed = 30.f;
const float AppSettings::camMinSpeed = 5.f;

AppSettings::AppSettings(glm::ivec3* cursorPos, float* cameraSpeed, world::Chunk* chunkPtr, openGL::SDLWinWrapper* winPtr, world::ControlPts* controlPts, world::PointsLights* pointLights) :
	_cursorPos(cursorPos),
	_cameraSpeed(cameraSpeed),
	_chunkPtr(chunkPtr),
	_winPtr(winPtr),
	_rayCastingEnable(false),
	_relativeMouse(SDL_FALSE),
	_nbRandomControlPts(1),
	_controlPts(controlPts),
	_pointLights(pointLights),
	_sunDir(1.0f, 0.0f, 1.0f),
	_sunColor(0.97f, 0.85f, 0.54f),
	_dayMode(1.0f),
	_animSun(false),
	_rbfLevel(0.03),
	_rbfAlpha(0.03),
	_rbf( std::function<float(float)>(std::bind(imath::rbf::terrainLvlQuadratic, std::placeholders::_1, _rbfAlpha, _rbfLevel))) {
	}

glm::ivec2 AppSettings::windowSize() const {
	glm::ivec2 size;
	SDL_GetWindowSize(_winPtr->getWindow(), &(size.x), &(size.y));
	return size;
}