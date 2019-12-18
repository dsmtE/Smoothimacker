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
	const float SPEED = 8.0f;
	const float SENSITIVITY = 0.2f;
	const float ZOOM = 45.0f;


	class Camera {

	private:
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
		Camera(const glm::vec3 &position = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3 &worldUp = glm::vec3(0.0f, 1.0f, 0.0f), const float &yaw = YAW, const float &pitch = PITCH);

		inline glm::mat4 getViewMatrix() const { return glm::lookAt(_position, _position + _front, _up); }
		glm::mat4 getProjectionMatrix(const float& screenWidth, const float& screenHeigh) const;
		inline float getZoom() const { return _zoom; }
		
		// input fonctions
        void handleEvents(const unsigned int &deltaTime);
		void handleRotationEvents(const SDL_Event &sdlEvent, const SDL_bool &relativeMouse);


	};
}
