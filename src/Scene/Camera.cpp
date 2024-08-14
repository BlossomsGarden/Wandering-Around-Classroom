#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>


// constructor with vectors
Camera::Camera(glm::vec3 position ,glm::vec3 up, float yaw, float pitch ) 
	: Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), Sensitivity(SENSITIVITY), Zoom(ZOOM)
{
	Position = position;
	WorldUp = up;
	Yaw = yaw;
	Pitch = pitch;
	UpdateCamera();
}
// constructor with scalar values
Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) 
	: Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), Sensitivity(SENSITIVITY), Zoom(ZOOM)
{
	Position = glm::vec3(posX, posY, posZ);
	WorldUp = glm::vec3(upX, upY, upZ);
	Yaw = yaw;
	Pitch = pitch;
	UpdateCamera();
}

void Camera::UpdateCamera()
{
	// calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);
	// also re-calculate the Right and Up vector
	Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Up = glm::normalize(glm::cross(Right, Front));
}

void Camera::ProcessMouseScroll(float yoffset)
{
	
	Zoom -= (float)yoffset;
	if (Zoom < 1.0f)
		Zoom = 1.0f;
	if (Zoom > 90.0f)
		Zoom = 90.0f;
	//std::cout << Zoom<<std::endl;
	//throw std::logic_error("The method or operation is not implemented.");
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset)
{
	xoffset *= Sensitivity;
	yoffset *= Sensitivity;

	Yaw += xoffset;
	Pitch += yoffset;
	if (PitchLock)
	{
		if (Pitch > PITCH_MAX)
			Pitch = PITCH_MAX;
		if (Pitch < PITCH_MIN)
			Pitch = PITCH_MIN;
	};
	UpdateCamera();
	//throw std::logic_error("The method or operation is not implemented.");
}

void Camera::ProcessKeyboard(KEYBOARD_ACTIONS direction,float delta_time)
{
	float velocity = MovementSpeed * delta_time;
	switch (direction)
	{
		case FORWARD:
			Position += Front * velocity;
			break;
		case BACKWARD:
			Position -= Front * velocity;
			break;
		case LEFT:
			Position -= Right * velocity;

			break;
		case RIGHT:
			Position += Right * velocity;
			break;
		case UP:
			Position += WorldUp * velocity;
			break;
		case DOWN :
			Position -= WorldUp * velocity;
			break;
		default:
		
			break;
	}


	//throw std::logic_error("The method or operation is not implemented.");
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(Position, Position + Front, Up);
}

void Camera::ProcessKeyboard(KEYBOARD_ACTIONS action)
{
	
}
