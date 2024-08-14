#pragma once
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/common.hpp>
#include "ControlledObject.h"



#define  ZOOM_MAX	45.0f
#define  ZOOM_MIN	1.0f

#define	 PITCH_MAX	89.0f
#define	 PITCH_MIN	-89.0f
//init with
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 25.0f;
const float SENSITIVITY = 0.1f;
//FOV
const float ZOOM = 45.0f;

//FPS camera using euler angle
class Camera : virtual public ControlledObject
{
private:



	
	glm::vec3 Position;		//camera pos
	glm::vec3 Front;		//front vec
	glm::vec3 Up;			//Up	vec
	glm::vec3 WorldUp;		//World UP vec
	glm::vec3 Right;		//Right vec


	float Yaw=YAW;
	float Pitch=PITCH;
	float MovementSpeed=SPEED;
	float Zoom = ZOOM;
	float Sensitivity=SENSITIVITY;
	bool  PitchLock = true;

	
	void UpdateCamera();
public:
	bool mode_switch = 0;
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
	void SetPitchLock(bool pitch_lock) { this->PitchLock = pitch_lock; };

	void ProcessMouseScroll(float yoffset) override;


	void ProcessMouseMovement(float xoffset, float yoffset) override;


	void ProcessKeyboard(KEYBOARD_ACTIONS direction,float delta_time) override;
	float GetFOV() { return this->Zoom; };
	glm::mat4 GetViewMatrix();
	glm::vec3 GetPos() const { return this->Position; };

	// Í¨¹ý ControlledObject ¼Ì³Ð
	void ProcessKeyboard(KEYBOARD_ACTIONS action) override;
};

