#include "Camera.hpp"

#include <iostream>

using namespace world;


Camera::Camera(const glm::vec3 &position, const glm::vec3 &worldUp, const float &yaw, const float &pitch) : 
    _position(position), _worldUp(worldUp), _yaw(yaw), _pitch(pitch), _movementSpeed(SPEED), _mouseSensitivity(SENSITIVITY), _zoom(ZOOM) {
	updateVectors();
}

void Camera::updateVectors() {
    // Calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    front.y = sin(glm::radians(_pitch));
    front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));


    _front = glm::normalize(front);
    // Also re-calculate the Right and Up vector
    _right = glm::normalize(glm::cross(_front, _worldUp));  // Normalize t
	_up = glm::normalize(glm::cross(_right, _front));
}


void Camera::handleEvent(unsigned int deltaTime, glm::vec2 deltaMouse, glm::vec2 deltaWheel) {

	bool hasChange = false;
	float velocity = _movementSpeed * deltaTime * 0.001f;

	const Uint8* state = SDL_GetKeyboardState(NULL); // get keys state
   
    
	// direction keyboard
	if (state[SDL_GetScancodeFromKey(SDLK_z)]) {
		_position += _front * velocity;
		hasChange = true;
	} else if (state[SDL_GetScancodeFromKey(SDLK_s)]) {
		_position -= _front * velocity;
		hasChange = true;
	}
	if (state[SDL_GetScancodeFromKey(SDLK_q)]) {
		hasChange = true;
		_position -= _right * velocity;
	} else if (state[SDL_GetScancodeFromKey(SDLK_d)]) {
		_position += _right * velocity;
		hasChange = true;
	}

	if (state[SDL_GetScancodeFromKey(SDLK_SPACE)]) {
		_position += _up * velocity;
	}else if (state[SDL_GetScancodeFromKey(SDLK_LSHIFT)]) {
		_position -= _up * velocity;
	}
	// zoom
	if (deltaWheel.y != 0) {
		_zoom -= deltaWheel.y ;
        if (_zoom <= 1.0f) _zoom = 1.0f;
        if (_zoom >= 70.0f) _zoom = 70.0f;
		hasChange = true;
	}

	// rotation with mouse
	if (deltaMouse.x != 0) {
		_yaw += deltaMouse.x * _mouseSensitivity;
		hasChange = true;
	}
	if (deltaMouse.y != 0) {
		_pitch -= deltaMouse.y * _mouseSensitivity;
		hasChange = true;
	}
	
	// update cam's vectors if needed
	if (hasChange) {
		updateVectors();
	}
}

glm::mat4 Camera::getProjectionMatrix(const float& screenWidth, const float& screenHeigh) const { 
	return glm::perspective(glm::radians(_zoom), screenWidth  / screenHeigh, 0.1f, 100.0f);
}