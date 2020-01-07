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
	const float SPEED = 25.0f;
	const float SENSITIVITY = 0.5f;
	const float ZOOM = 45.0f;

	 ///\brief class camera using to move in our scene
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

	   /**
	    * @brief Calculates the front vector from the Camera's with Euler Angles
	    */

	   ///
	   
		void updateVectors();

	public:
		 ///\brief Construct a new Camera object
		 ///
		 ///\param screenWidth
		 ///\param screenHeigh
		 ///\param position : the position of our camera in world space
		 ///\param worldUp
		 ///\param yaw : yaw angle in degree
		 ///\param pitch : pitch angle in degree
		Camera(const int &screenWidth, const int &screenHeigh, 
				const glm::vec3 &position = glm::vec3(0.0f, 0.0f, 0.0f), 
				const glm::vec3 &worldUp = glm::vec3(0.0f, 1.0f, 0.0f), 
				const float &yaw =  45.0f, 
				const float &pitch = -10.0f);

		 ///\brief return the view matrix used in  shaders
		 ///\return glm::mat4 
		glm::mat4 viewMatrix() const;
		///
		 ///\brief retur the projection matrix used in shaders
		 ///\return glm::mat4 
		glm::mat4 projectionMatrix() const;
		///
		 ///\brief return the computed projection View matrix used in shaders
		 ///\return glm::mat4 
		glm::mat4 PVMatrix() const;

		 ///\brief Get the Zoom value
		 ///\return float 
		inline float getZoom() const { return _zoom; }

		///\brief Get the Front dir vecotr
		///\return glm::vec3 
		inline glm::vec3 getFrontDir() const { return _front; }

		///\brief Get the Camera Speed pointer
		///\return float* 
		inline float* getCameraSpeedPtr() { return &_movementSpeed; };

		///\brief Get the Pos of our camera
		///\return glm::vec3 
		inline glm::vec3 getPos() const { return _position; };

		///\brief Set the new size for projection matrix
		///\param size
		void setSize(const glm::ivec2 &size);
		// input fonctions
		
		///\brief delegated function that manages the camera's movements events
		///\param deltaTime time between two frames used to normalize movements 
        void handleEvents(const unsigned int &deltaTime);

		///\brief elegated function that manages the camera's keyboard & mouse events
		///\param sdlEvent
		///\param relativeMouse
		void handleRotationEvents(const SDL_Event &sdlEvent, const SDL_bool &relativeMouse);


	};
}
