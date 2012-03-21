#include "WPILib.h"
//#include <math.h>

// Constants.h has the mappings for PWM, Digital IO Channels, etc.
#include "Constants.h"
#include "Sweeper.h"
#include "ToggleButton.h"
#include "TriggerWheel.h"
#include "AutonomousConstants.h"
#include "FlexibleScaler.h"
#include "BallCollectionSystem.h"
#include <stdio.h>

#define TEST_MODE

//

class RobotDemo : public SimpleRobot
{

	RobotDrive *leroyDrive; // robot drive system
	Joystick *joystickOne;
	Joystick *joystickTwo;
	FlexibleScaler *tankDriveScaler;
	
	// Encoder on right wheel. Left wheel encoder is broken and not needed.
	Encoder *driveEncoder;
	
	BallCollectionSystem *ballCollector;
	
	

public:
	RobotDemo(void);
	
	void TankDriveMe(void);
	
	void AutonDriveInches(float inches, float speed);

	
	void SimpleAutonOne(void);
	
	// These are found in TestMode.cpp
    void TestModeSetup(void);
    void TestModeControlLoop(void);
    
    void Autonomous(void);
    void OperatorControl(void);
	

};

