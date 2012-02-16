#import "WPILib.h"

void testButtons(Joystick *joystick){
	
	for (int i=1; i<17; i++){
		if (joystick->GetRawButton(i))
			printf("Button %d pushed.\n", i);
	}
	
}

void testAxis(Joystick *joystick){
	// need to slam stick past 0.2 to register
	
	for (int i=1; i<6; i++)
		if (joystick->GetRawAxis(i)>0.2)
			printf("Stick %d is at %f\n", i, joystick->GetRawAxis(i));
	
	
}