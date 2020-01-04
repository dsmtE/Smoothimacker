#pragma once 

#include <glm/glm.hpp>
#include <SDL.h>
#include "openGL/SDLWinWrapper.hpp"
class AppSettings {
private:

	glm::ivec3 *_cursorPos;
	float* _cameraSpeed;
	openGL::SDLWinWrapper* _winPtr;

	// others setting attributs
	bool _rayCastingEnable;
	SDL_bool _relativeMouse; // handle mouse rotation with mouse

	// light settings
	glm::vec3 _sunDir;
	glm::vec3 _sunColor;

public:

	//static attributs
	const static float camMinSpeed;
	const static float camMaxSpeed;

	AppSettings(glm::ivec3* cursorPos, float* cameraSpeed, openGL::SDLWinWrapper* winPtr);
	~AppSettings() = default;

	// getters expose app setting 
	inline glm::ivec3* cursorPos() { return _cursorPos; };
	inline float* CameraSpeed() { return _cameraSpeed; };
	inline bool& rayCasting() { return _rayCastingEnable; };
	inline SDL_bool& relativeMouse() { return _relativeMouse; };
	inline glm::vec3& sunColor() { return _sunColor; }; 
	inline glm::vec3& sunDir() { return _sunDir; };

	glm::ivec2 windowSize() const;
	inline void exit() { _winPtr->exit(); };

};