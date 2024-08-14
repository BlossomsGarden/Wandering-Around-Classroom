#include "Controller.h"
#include "../utils.h"
#include <iostream>




Controller* Controller::THIS_PTR = NULL;

Controller::Controller(ControlledObject* obj, GLFWwindow* window) :dst(obj), window(window)
{
	//GLFW can't register callback function with this pointer(object), so use the class's static function and set This pointer manually 
	BindThis();
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetScrollCallback(window, Controller::ScrollCallback);
	glfwSetFramebufferSizeCallback(window, Controller::FrameBufferSizeCallback);
	glfwSetCursorPosCallback(window, Controller::MouseCallback);
}

Controller::~Controller()
{
	UnbindThis();
}

void Controller::FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	if (THIS_PTR != NULL) {

		GLCALL(glViewport(0, 0, width, height));
	}
	else {
		std::cout << "[ERROR]Controller.cpp:THIS pointer is not bind to any controller object.";
	}
}

void Controller::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (THIS_PTR != NULL) {

		THIS_PTR->dst->ProcessMouseScroll((float)yoffset);
	}
	else {
		std::cout << "[ERROR]Controller.cpp:THIS pointer is not bind to any controller object.";
	}
}

void Controller::MouseCallback(GLFWwindow* window, double xposIn, double yposIn)
{
	if (THIS_PTR != NULL) {
		float xpos = static_cast<float>(xposIn);
		float ypos = static_cast<float>(yposIn);

		if (THIS_PTR->firstmouse)
		{
			THIS_PTR->lastX = xpos;
			THIS_PTR->lastY = ypos;
			THIS_PTR->firstmouse = false;
		}

		float xoffset = xpos - THIS_PTR->lastX;
		float yoffset = THIS_PTR->lastY - ypos; // reversed since y-coordinates go from bottom to top

		THIS_PTR->lastX = xpos;
		THIS_PTR->lastY = ypos;

		THIS_PTR->dst->ProcessMouseMovement(xoffset, yoffset);
	}
	else {
		std::cout << "[ERROR]Controller.cpp:THIS pointer is not bind to any controller object.";
	}
}

void Controller::ProcessInput(float deltaTime)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		dst->ProcessKeyboard(KEYBOARD_ACTIONS::FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		dst->ProcessKeyboard(KEYBOARD_ACTIONS::BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		dst->ProcessKeyboard(KEYBOARD_ACTIONS::LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		dst->ProcessKeyboard(KEYBOARD_ACTIONS::RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		dst->ProcessKeyboard(KEYBOARD_ACTIONS::UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		dst->ProcessKeyboard(KEYBOARD_ACTIONS::DOWN, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		dst->ProcessKeyboard(KEYBOARD_ACTIONS::SWITCHSPOTLIGHT);
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		dst->ProcessKeyboard(KEYBOARD_ACTIONS::MODE_SWITCH);

}

void Controller::SetInputMode(int mode, int value)
{
	glfwSetInputMode(window, mode, value);
}
