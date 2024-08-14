#pragma once

enum KEYBOARD_ACTIONS {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN,
	SWITCHSPOTLIGHT,
	MODE_SWITCH
};

/*interface for controller*/
class ControlledObject {
public:
	virtual void ProcessMouseScroll(float yoffset)=0;
	virtual void ProcessMouseMovement(float xoffset, float yoffset)=0;
	virtual void ProcessKeyboard(KEYBOARD_ACTIONS direction,float delta_time)=0;
	virtual void ProcessKeyboard(KEYBOARD_ACTIONS action) = 0;
};
