#include "ToggleButton.h"
#include "TriggerWheel.h"

extern void testButtons(Joystick *joystick);
extern void testAxis(Joystick *joystick);


class TestHarness
{
public:
	
	Victor *victorOne;
	Victor *victorTwo;
	Joystick *joystick;
	ToggleButton *toggleButton;
	TriggerWheel *wheel;
	DigitalInput *digInput;
	
	float victorOneValue;
	float victorTwoValue;
	
	TestHarness(Joystick *js);
	
	void Update(void);	
	
};
