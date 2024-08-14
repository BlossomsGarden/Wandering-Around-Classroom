#pragma once
#include "ControlledObject.h"
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include "..\Global.h"



//TODO: Better way to address the callback func issue: now using explicit this pointer
/*BEWARE OF THE STATIC THIS POINTER
MAKE SURE YOU BIND IT TO THE RIGHT PLACE*/
class Controller
{
private:
	ControlledObject* dst;
	GLFWwindow* window;


	bool firstmouse = true;
	float lastX = SCREEN_WIDTH / 2.0f;
	float lastY = SCREEN_HEIGHT / 2.0f;
	
public:
	/*Explicitly direct THIS pointer*/
	static Controller* THIS_PTR;
	Controller(ControlledObject* obj, GLFWwindow* window);
	~Controller();
	static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
	static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	static void MouseCallback(GLFWwindow* window, double xposIn, double yposIn);


	void BindObject(ControlledObject* const dst) { this->dst = dst; };
	void UnbindObject() { this->dst = nullptr; };
	void BindWindow(GLFWwindow* window) { this->window = window; };
	void UnbindWindow() { this->window = nullptr; };
	//calculate delta time externally
	void ProcessInput(float deltaTime);
	
	
	void SetInputMode(int mode, int value);
	void BindThis() { if(this!=NULL) THIS_PTR = this; };
	void UnbindThis() { THIS_PTR = NULL; };
};


