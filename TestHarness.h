#include "ToggleButton.h"
#include "TriggerWheel.h"

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
