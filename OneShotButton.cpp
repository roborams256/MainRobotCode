#include "OneShotButton.h"

OneShotButton::OneShotButton(Joystick *jstick, int rawButton){
	
	button = rawButton;
	joystick = jstick;
	
	// if button is up, we are armed
	if (!joystick->GetRawButton(button))
		armed = true;
	else
		armed = false;
	
	
}

bool OneShotButton::HasFired(void){
	
	bool pressed = joystick->GetRawButton(button);
	
	if (armed && pressed){
		armed = false;
		return true;
	}
	
	if (!armed && !pressed){
		armed = true;
		return false;
	}
	
	if (!armed && pressed){
		return false;
	}
	
	if (armed && !pressed)
		return false;
	
}

void OneShotButton::ForceArmed(){
	
	armed = true;
}