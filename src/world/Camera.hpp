#pragma once

#include <SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include "../openGL/glException.hpp"
#include <cmath>

namespace world {

// Euler angles
// 
//        (Y)( YAW )
//         ^
//         |
//         .---->(X) ( PITCH )
//        /
//       v
//     (Z) ( ROLL ) 

	// Default camera values
	const float YAW = -90.0f;
	const float PITCH = 0.0f;
	const float SPEED = 25.0f;
	const float SENSITIVITY = 0.5f;
	const float ZOOM = 45.0f;

	class Camera {

	private:

		int _screenWidth;
		int _screenHeigh;

		glm::vec3 _position;
		glm::vec3 _worldUp;

		float _yaw;
		float _pitch;

		float _movementSpeed;
		float _mouseSensitivity;
		float _zoom; // for  field of view

        glm::vec3 _front;
        glm::vec3 _up;
        glm::vec3 _right;

		bool _hasChanged;
		SDL_bool _enableCamRotation;

	   // Calculates the front vector from the Camera's with Euler Angles
		void updateVectors();

	public:
		// Constructor with vectors
		Camera(const int &screenWidth, const int &screenHeigh, const glm::vec3 &position = glm::vec3(0.0f, 0.0f, 0.0f), 
				const glm::vec3 &worldUp = glm::vec3(0.0f, 1.0f, 0.0f), 
				const float &yaw = YAW, 
				const float &pitch = PITCH);

		glm::mat4 viewMatrix() const;
		// glm::mat4 projectionMatrix(const int& screenWidth, const int& screenHeigh) const;
		glm::mat4 projectionMatrix() const;
		// glm::mat4 PVMatrix(const int& screenWidth, const int& screenHeigh) const;
		glm::mat4 PVMatrix() const;
		inline float getZoom() const { return _zoom; }
		inline glm::vec3 getFrontDir() const { return _front; }
		inline float* getCameraSpeedPtr() { return &_movementSpeed; };
		inline glm::vec3 getPos() const { return _position; };
		// input fonctions
        void handleEvents(const unsigned int &deltaTime);
		void handleRotationEvents(const SDL_Event &sdlEvent, const SDL_bool &relativeMouse);


	};
}
