#include "WPILib.h"
#include "Constants.h"
#include "Sweeper.h"
#include "ToggleButton.h"
#include "TriggerWheel.h"
#include "AutonomousConstants.h"
#include "FlexibleScaler.h"
#include "BallCollectionSystem.h"
#include "BallCannon.h"
#include <stdio.h>

//#define TEST_MODE




class RobotDemo : public SimpleRobot
{

	RobotDrive *leroyDrive; // robot drive system
	Joystick *joystickOne;
	Joystick *joystickTwo;
	FlexibleScaler *tankDriveScaler;
	
	// Encoder on right wheel. Left wheel encoder is broken and not needed.
	Encoder *driveEncoder;
	
	BallCollectionSystem *ballCollector;
	BallCannon *ballCannon;
	
	enum TestModes {
		kBallCollector,
		kBallShooter,
		kDriveTrain,
		kAllSensors,
		kUltrasonic,
		kJoysticks,
		kBridgeActuator,
		kTestEnd
	};
	
#ifdef TEST_MODE
	TestModes testMode;
	ToggleButton *testModeToggleButton;
#endif
	
	

public:
	RobotDemo(void){
		
		leroyDrive = new RobotDrive(PWM_LEFT_DRIVE, PWM_RIGHT_DRIVE);
		leroyDrive->SetExpiration(1.1);
		
		joystickOne = new Joystick(1);
		joystickTwo = new Joystick(2);
		
		// The FlexibleScaler object provides 4 different ways of scaling the joystick inputs
		// to provide different "feels" and limits for the joystick. In this one, we are using
		// a parabolic curve (smooth at the bottom, slam hard near the top) and a limit of 75%.
		tankDriveScaler = new FlexibleScaler(FlexibleScaler::kScaleLinear, 0.85);
		
		driveEncoder = new Encoder(DIO_ENCODER_A, DIO_ENCODER_B, true, CounterBase::k4X);
		
		driveEncoder->Reset();
		driveEncoder->SetDistancePerPulse(INCHES_PER_TICK);
		driveEncoder->Start();
		
		// Initialize the ball collection system
		
		ballCollector = new BallCollectionSystem();
		ballCannon = new BallCannon();
		
		
#ifdef TEST_MODE
		
		testModeToggleButton = new ToggleButton(joystickOne, LB_BUTTON, (int)kTestEnd);
	
#endif
		
};
	
	
	
void TankDriveMe(){
		
		
		//float rdrive = tankDriveScaler->Scale(joystickOne->GetRawAxis(LEFT_Y_AXIS));
		//float ldrive = tankDriveScaler->Scale(joystickOne->GetRawAxis(RIGHT_Y_AXIS));
		
		float rdrive = 0.75*joystickOne->GetRawAxis(LEFT_Y_AXIS);
		float ldrive = 0.75*joystickOne->GetRawAxis(RIGHT_Y_AXIS);
				
		DEBUG_PRINT("Left drive:[%f] | Right drive:[%f]\n", ldrive, rdrive);		
	    
		leroyDrive->TankDrive(ldrive, rdrive);
	    
};
	
void AutonDriveInches(float inches, float speed){
		
		leroyDrive->Drive(speed, AUTON_TIMED_CURVE_OFFSET); 	// drive forwards 30% speed
		
		while ( driveEncoder->GetDistance() < inches ){
					DEBUG_PRINT("Roto %lf\n", driveEncoder->GetDistance());
				}
								
		leroyDrive->Drive(0.0, 0.0); 	// stop robot

};

	
void SimpleAutonOne(void){
		
		// This is the simplest autonomous routine without multithreading
		// TODO: Run these concurrently using thread to save time, if needed
		
		ballCollector->On();
		ballCannon->SetPower(0.35);
		
		AutonDriveInches( DISTANCE_FROM_KEY_TO_WALL - DISTANCE_BUMPER_WIDTH, 0.30 );
		
		ballCollector->FireAuto();
		
}
	
	
void Autonomous(void){
		
		leroyDrive->SetSafetyEnabled(false);
		
		SimpleAutonOne();
		
}

void OperatorModeSetup(void){
		
		DEBUG_PRINT("Normal operator mode. In setup.\n");
}



void OperatorModeControlLoop(void){
		
	TankDriveMe();
	
	ballCannon->DirectDriveAngle(joystickTwo->GetRawAxis(LEFT_Y_AXIS));
	
	if ( joystickTwo->GetRawButton(A_BUTTON) ){
		ballCannon->SetPower(0.35);
		ballCollector->FireAuto();
	} 
	else if ( joystickTwo->GetRawButton(B_BUTTON) ){
		ballCannon->SetPower(0.50);
		ballCollector->FireAuto();
	} 
	else if ( joystickTwo->GetRawButton(Y_BUTTON) ){
		ballCannon->SetPower(0.65);
		ballCollector->FireAuto();
	} 
	else {
		ballCollector->Hold();
	}
			
	
}
	

void OperatorControl(void){
		
		DEBUG_PRINT("Starting operator controlled mode.\n");
		
		// TODO: What do we need to do with this?
		leroyDrive->SetSafetyEnabled(false);
		
#ifdef TEST_MODE
		printf("Test mode!\n");
		TestModeSetup();
#endif
		
#ifndef TEST_MODE
		
		
		OperatorModeSetup();
		
#endif
		
		
		
		//leroyDrive->SetSafetyEnabled(true);
		while (IsOperatorControl())
		{
			
#ifdef TEST_MODE
			
		TestModeControlLoop();
#endif

#ifndef TEST_MODE
		
		OperatorModeControlLoop();
#endif
		
			
			Wait(0.005);				// wait for a motor update time
		} // end of operator control loop
}


/*
 * 
 * 
 * TEST CODE BELOW
 * 
 * 
 * 
 */

#ifdef TEST_MODE

void TestModeSetup(void){
	
	printf("Test setup some shit\n");
	
	testMode = kBallCollector;
};

void TestModeControlLoop(void){
	
	int mode = testModeToggleButton->State();
	
	switch ((TestModes)mode){
	
	/*
	 *  BallCollectorsystem Test Code 
	 * 
	 */
	
	case kBallCollector:
	{
		
		// Must call update method so Fire() will work
		ballCollector->Update();
		
		// Button A turns on
		if (joystickOne->GetRawButton(A_BUTTON)){
			DEBUG_PRINT("Turning on ball collector!\n");
			ballCollector->On();
			return;
		}
		
		// Button B is off
		if (joystickOne->GetRawButton(B_BUTTON)){
			DEBUG_PRINT("Turning off ball collector!\n");
			ballCollector->Off();
			return;
		}
		
		if (joystickOne->GetRawButton(X_BUTTON)){
			DEBUG_PRINT("Reversing ball collector!\n");
			ballCollector->Reverse();
			return;
		}
		
		if (joystickOne->GetRawButton(Y_BUTTON)){
					DEBUG_PRINT("Firing ball collector!\n");
					ballCollector->On();
					ballCollector->Fire();
					return;
				}
		
		if (joystickOne->GetRawButton(START_BUTTON)){
					DEBUG_PRINT("Help: A=ON, B=Off, C=Reverse, D=Fire\n");
					ballCollector->On();
					ballCollector->Fire();
					return;
				}
		
		ballCollector->Off(); // No button, do nothing
		break;
	};
	
	case kDriveTrain:
		{
			
			if (joystickOne->GetRawButton(A_BUTTON)){
				DEBUG_PRINT("Switching to unscaled drive!\n");
				tankDriveScaler->SetModeAndScaler(FlexibleScaler::kScaleUnity, 1.0);
			}
			
			if (joystickOne->GetRawButton(B_BUTTON)){
				DEBUG_PRINT("Switching to scaled drive @40pct!\n");
				tankDriveScaler->SetModeAndScaler(FlexibleScaler::kScaleLinear, 0.4);

			}
			
			if (joystickOne->GetRawButton(X_BUTTON)){
				DEBUG_PRINT("Switching to log drive!\n");
				tankDriveScaler->SetModeAndScaler(FlexibleScaler::kScalePower2, 1.0);
			}
					
			if (joystickOne->GetRawButton(Y_BUTTON)){
				DEBUG_PRINT("Switching to log/scale drive @75pct!\n");
				tankDriveScaler->SetModeAndScaler(FlexibleScaler::kScalePower2AndLinear, 0.75);
			}			
					
			TankDriveMe();
			break;
		}
		
	default:
		break;
		
	
	};
	
};

#endif // TEST_CODE
		
};


START_ROBOT_CLASS(RobotDemo);
