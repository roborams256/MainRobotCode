#include "WPILib.h"

/*
 * 
 */

class OneShotButton{
	
	bool armed;
	Joystick *joystick;
	int button;
	
public:
	
	
	
	OneShotButton(Joystick *jstick, int rawButton);
	bool HasFired();
	void ForceArmed();
};
