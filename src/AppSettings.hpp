#pragma once 

#include <glm/glm.hpp>
#include <SDL.h>
#include "openGL/SDLWinWrapper.hpp"
#include "world/ControlPts.hpp"
#include "world/Chunk.hpp"
#include "world/PointsLights.hpp"
#include "imath/RadialBasisFunction.hpp"

class AppSettings {

public:

	glm::ivec3 *_cursorPos;
	float* _cameraSpeed;
	openGL::SDLWinWrapper *_winPtr;
	world::Chunk *_chunkPtr;
	world::ControlPts *_controlPts;
	world::PointsLights *_pointLights;

	// others setting attributs
	bool _rayCastingEnable;
	SDL_bool _relativeMouse; // handle mouse rotation with mouse
	int _nbRandomControlPts; // nb control pt to be generated

	// light settings
	glm::vec3 _sunDir;
	glm::vec3 _sunColor;
	float _dayMode;
	bool _animSun;

	//radial basis functions
	float _rbfLevel;
	float _rbfAlpha;
	float _rbfsinF;
	std::function< float(float)> _rbf;

	//color setting
	glm::vec3 _colorPick;

	//static attributs
	const static float camMinSpeed;
	const static float camMaxSpeed;

	AppSettings(glm::ivec3* cursorPos, float* cameraSpeed, world::Chunk *chunkPtr, openGL::SDLWinWrapper* winPtr, world::ControlPts *controlPts, world::PointsLights *pointLights);
	~AppSettings() = default;

	glm::ivec2 windowSize() const;
	inline void exit() { _winPtr->exit(); };

};