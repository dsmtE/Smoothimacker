#include "Camera.hpp"

#include <iostream>

using namespace world;

//        (Y) ^(Z)
//         ^ /
//         |/
//         .---->(X)

Camera::Camera(const int &screenWidth, const int &screenHeigh, const glm::vec3 &position, const glm::vec3 &worldUp, const float &yaw, const float &pitch) : 
    _screenWidth(screenWidth), _screenHeigh(screenHeigh), _position(position), _worldUp(worldUp), _yaw(yaw), _pitch(pitch), _movementSpeed(SPEED), 
	_mouseSensitivity(SENSITIVITY), _zoom(ZOOM), _hasChanged(false) {
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

void Camera::handleEvents(const unsigned int &deltaTime) {

	float velocity = _movementSpeed * deltaTime * 0.001f;
	const Uint8* state = SDL_GetKeyboardState(NULL); // get keys state
    
	// direction keyboard
	if (state[SDL_GetScancodeFromKey(SDLK_z)]) {
		_position += _front * velocity;
		_hasChanged = true;
	}
	if (state[SDL_GetScancodeFromKey(SDLK_s)]) {
		_position -= _front * velocity;
		_hasChanged = true;
	}
	if (state[SDL_GetScancodeFromKey(SDLK_q)]) {
		_hasChanged = true;
		_position -= _right * velocity;
	}
	
	if (state[SDL_GetScancodeFromKey(SDLK_d)]) {
		_position += _right * velocity;
		_hasChanged = true;
	}

	if (state[SDL_GetScancodeFromKey(SDLK_SPACE)]) {
		_position += _worldUp * velocity;
	}
	if (state[SDL_GetScancodeFromKey(SDLK_LSHIFT)]) {
		_position -= _worldUp * velocity;
	}

	// update cam's vectors if needed
	if (_hasChanged) {
		updateVectors();
	}

}

void Camera::handleRotationEvents(const SDL_Event &sdlEvent, const SDL_bool &relativeMouse) {

	switch (sdlEvent.type) {
		case SDL_MOUSEWHEEL:
			glm::vec2 deltaWheel = glm::ivec2(sdlEvent.wheel.x, sdlEvent.wheel.y);

			// zoom for projection matrix
			if (deltaWheel.y != 0) {
				_zoom -= deltaWheel.y ;
				if (_zoom <= 1.0f) _zoom = 1.0f;
				if (_zoom >= 70.0f) _zoom = 70.0f;
				_hasChanged = true;
			}
			
			break;

		case SDL_MOUSEMOTION:
			if(relativeMouse == SDL_TRUE || _enableCamRotation == SDL_TRUE) {
				glm::vec2 deltaMouse = glm::ivec2(sdlEvent.motion.xrel, sdlEvent.motion.yrel);

				// rotation with mouse
				if (deltaMouse.x != 0) {
					_yaw += deltaMouse.x * _mouseSensitivity;
					_hasChanged = true;
				}
				if (deltaMouse.y != 0) {
					_pitch -= deltaMouse.y * _mouseSensitivity;
					_hasChanged = true;
				}
			}
			break;

		case SDL_MOUSEBUTTONDOWN:
			if(sdlEvent.button.button == SDL_BUTTON_MIDDLE) {
				_enableCamRotation = SDL_TRUE;
			}
			break;

		case SDL_MOUSEBUTTONUP:
			if(sdlEvent.button.button == SDL_BUTTON_MIDDLE) {
				_enableCamRotation = SDL_FALSE;
			}
			break;

		default:
			break;
	}
}


glm::mat4 Camera::PVMatrix() const {
	return projectionMatrix() * viewMatrix();
}

glm::mat4 Camera::projectionMatrix() const { 
	return glm::perspective(glm::radians(_zoom), float(_screenWidth)  / float(_screenHeigh), 0.1f, 300.0f);
}

glm::mat4 Camera::viewMatrix() const { 
	return glm::lookAt(_position, _position + _front, _up); 
}