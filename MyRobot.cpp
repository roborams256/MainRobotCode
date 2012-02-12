#include "WPILib.h"

// Constants.h has the mappings for PWM, Digital IO Channels, etc.
#include "Constants.h"
#include "Sweeper.h"
#include "ToggleButton.h"
#include "TriggerWheel.h"
#include "TestHarness.h"

//#define TEST_MODE

#include <stdio.h>

/**
 * This is a demo program showing the use of the RobotBase class.
 * The SimpleRobot class is the base of a robot application that will automatically call your
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on
 * the driver station or the field controls.
 */ 
class RobotDemo : public SimpleRobot
{

	RobotDrive *leroyDrive; // robot drive system
	Joystick *joystickOne;
	Joystick *joystickTwo;
	Sweeper *sweeper;

public:
	RobotDemo(void){
		leroyDrive = new RobotDrive(PWM_LEFT_DRIVE, PWM_RIGHT_DRIVE);
		
		sweeper = new Sweeper(SPIKE_SWEEPER);
		leroyDrive->SetExpiration(1.1);
		
		joystickOne = new Joystick(1);

	}

	/**
	 * Drive left & right motors for 2 seconds then stop
	 */
	void Autonomous(void)
	{

		leroyDrive->SetSafetyEnabled(false);
		leroyDrive->Drive(0.5, 0.0); 	// drive forwards half speed
		Wait(2.0); 				//    for 2 seconds
		leroyDrive->Drive(0.0, 0.0); 	// stop robot

	}

	/**
	 * Runs the motors with arcade steering. 
	 */
	void OperatorControl(void)
	{
		leroyDrive->SetSafetyEnabled(false);
		printf("Starting op con!");
		
#ifdef TEST_MODE
		printf("Test mode!\n");
		TestHarness *testHarness = new TestHarness(joystickOne);
#endif
		
#ifndef TEST_MODE
		
		Relay *belt = new Relay(3);
		belt->Set(Relay::kReverse);
		
		//Relay *trigger = new Relay(SPIKE_TRIGGER);
		//trigger->Set(Relay::kForward);
		TriggerWheel *trigger = new TriggerWheel(1, 1);
		
		Jaguar *bjr = new Jaguar(10);
		Jaguar *bjl = new Jaguar(9);
		
		bjl->Set(-0.35);
		bjr->Set(-0.35);
		
#endif
		
		
		//leroyDrive->SetSafetyEnabled(true);
		while (IsOperatorControl())
		{
			
#ifndef TEST_MODE
			sweeper->On();
			// Use left and right joysticks on Gamepad One
		    leroyDrive->TankDrive(joystickOne->GetRawAxis(GPAD_LEFT_Y_RAW_AXIS) * JOYSTICK_DRIVE_SCALE_FACTOR, 
		    		joystickOne->GetRawAxis(GPAD_RIGHT_Y_RAW_AXIS) * JOYSTICK_DRIVE_SCALE_FACTOR); 
		    
		    if (joystickOne->GetRawButton(2))
		    	trigger->fireSemiAuto();
		    
		    trigger->Update();
		    
#endif
#ifdef TEST_MODE
		    
		    testHarness->victorOneValue = joystickOne->GetRawAxis(GPAD_LEFT_Y_RAW_AXIS);
		    testHarness->Update();
#endif

			Wait(0.005);				// wait for a motor update time
		}
	}
};

START_ROBOT_CLASS(RobotDemo);

