#include "WPILib.h"
#include "Constants.h"
#include "Sweeper.h"
#include "ToggleButton.h"
#include "TriggerWheel.h"
#include "AutonomousConstants.h"
#include "FlexibleScaler.h"
#include "BallCollectionSystem.h"
#include "BallCannon.h"
#include "BridgeActuator.h"
#include <stdio.h>
#include <queue>

//#define TEST_MODE

void testButtons(Joystick *joystick);

void testAxis(Joystick *joystick);


class RobotDemo : public SimpleRobot
{

	DriverStationLCD *dsLCD;
	RobotDrive *leroyDrive; // robot drive system
	Joystick *joystickOne;
	Joystick *joystickTwo;
	FlexibleScaler *tankDriveScaler;
	
	// Encoder on right wheel. Left wheel encoder is broken and not needed.
	Encoder *driveEncoder;
	
	BallCollectionSystem *ballCollector;
	BallCannon *ballCannon;
	
	BridgeActuator *bridgeSlapper;
	
	float lDriveSum;
	float rDriveSum;
	int lDriveWindow;
	int rDriveWindow;
	queue<float> lDriveQueue;
	queue<float> rDriveQueue;
	
	enum TestModes {
		kRawIO,
		kDriveTrain,
		kBallCollector,
		kBallCannon,
		kBridgeActuator,
		kAllSensors,
		kUltrasonic,
		kJoysticks,		
		kTestEnd
	};
	
#ifdef TEST_MODE
	TestModes testMode;
	ToggleButton *testModeToggleButton;
	ToggleButton *testSubModeToggleButton;
	DigitalInput *dinA;
	DigitalInput *dinB;
	Jaguar *testJag;
	Relay *testRelay;
	bool startTest;
	int subTestMode, lastSubTestMode;
	

	
#endif
	
	

public:
	RobotDemo(void){
		
		leroyDrive = new RobotDrive(PWM_LEFT_DRIVE, PWM_RIGHT_DRIVE);
		leroyDrive->SetExpiration(1.1);
		
		joystickOne = new Joystick(1);
		joystickTwo = new Joystick(2);
		
		dsLCD = DriverStationLCD::GetInstance();
		
		// The FlexibleScaler object provides 4 different ways of scaling the joystick inputs
		// to provide different "feels" and limits for the joystick. In this one, we are using
		// a parabolic curve (smooth at the bottom, slam hard near the top) and a limit of 75%.
		tankDriveScaler = new FlexibleScaler(FlexibleScaler::kScaleLinear, 0.85);
		
		driveEncoder = new Encoder(DIO_ENCODER_A, DIO_ENCODER_B, true, CounterBase::k4X);
		
		driveEncoder->Reset();
		driveEncoder->SetDistancePerPulse(INCHES_PER_TICK);
		driveEncoder->Start();
		
		lDriveSum = 0;
		rDriveSum = 0;
		lDriveWindow = LDRIVEWINDOW;
		rDriveWindow = RDRIVEWINDOW;
		deque<float> lqinit (lDriveWindow, 0);
		deque<float> rqinit (rDriveWindow, 0);
		lDriveQueue = queue<float>(lqinit);
		rDriveQueue = queue<float>(rqinit);
		//for (int i = 0; i < lDriveWindow; i++) lDriveQueue.push(0); //initialize these for SMOOOTHNESS
		//for (int i = 0; i < rDriveWindow; i++) rDriveQueue.push(0);
		
		// Initialize the ball collection system
		
#ifndef TEST_MODE
		ballCollector = new BallCollectionSystem();
		ballCannon = new BallCannon();
		bridgeSlapper = new BridgeActuator();
		
		
#endif
		
		
		
#ifdef TEST_MODE
		
		testModeToggleButton = new ToggleButton(joystickOne, LB_BUTTON, (int)kBridgeActuator);
		testSubModeToggleButton = new ToggleButton(joystickOne, RB_BUTTON, 6);
		testJag = NULL;
		testRelay = NULL;
		lastSubTestMode = 0;
		ballCollector = NULL;
		ballCannon = NULL;
		bridgeSlapper = NULL;
		dinA = new DigitalInput(DIG_IN_TOP_SENSOR);
		dinB = new DigitalInput(DIG_IN_BOTTOM_SENSOR);
				
	
#endif
		
};
	
/*void lUpdateDrive(int joyVal)
{
	float qv = lDriveQueue.pop() || 0;
	lDriveSum = ((lDriveSum * lDriveWindow) - qv + joyVal) / lDriveWindow; 
	lDriveQueue.push(joyVal);
};

void rUpdateDrive(int joyVal)
{
	float qv = rDriveQueue.pop() || 0;
	rDriveSum = ((rDriveSum * rDriveWindow) - qv + joyVal) / rDriveWindow; 
	rDriveQueue.push(joyVal);
};*/
	
void TankDriveMe(){
		
		
		//float rdrive = tankDriveScaler->Scale(joystickOne->GetRawAxis(LEFT_Y_AXIS));
		//float ldrive = tankDriveScaler->Scale(joystickOne->GetRawAxis(RIGHT_Y_AXIS));
		
		float lJoyVal = 0.85*joystickOne->GetRawAxis(LEFT_Y_AXIS);
		float rJoyVal = 0.85*joystickOne->GetRawAxis(RIGHT_Y_AXIS);
	
		//rUpdateDrive(lJoyVal);
		//lUpdateDrive(rJoyVal);
	
		float rdrive = lJoyVal;//rDriveSum;//0.85*joystickOne->GetRawAxis(LEFT_Y_AXIS);
		float ldrive = rJoyVal;//lDriveSum;//0.85*joystickOne->GetRawAxis(RIGHT_Y_AXIS);
				
		DEBUG_PRINT("Left drive:[%f] | Right drive:[%f]\n", ldrive, rdrive);		
	    
		leroyDrive->TankDrive(-ldrive, -rdrive); /* changing direction sense */
	    
};
	
void AutonDriveInches(float inches, float speed){
		
		leroyDrive->Drive(speed, AUTON_TIMED_CURVE_OFFSET); 	
		
		while ( driveEncoder->GetDistance() < inches ){
			dsLCD->Printf(DriverStationLCD:: kUser_Line1, 1, "Auton: %1.2f inches");
			dsLCD->UpdateLCD();
			}
								
		leroyDrive->Drive(0.0, 0.0); 	// stop robot

};

	
void SimpleAutonOne(void){
		
		// This is the simplest autonomous routine without multithreading
		// TODO: Run these concurrently using thread to save time, if needed
		
		ballCollector->On();
		ballCannon->SetPower(AUTON_BALL_POWER);
		ballCannon->Calibrate();
		
		
		
		while (ballCannon->calibrating)
			ballCannon->Update();
		
		
		Wait(2.0);
		
		//AutonDriveInches( DISTANCE_FROM_KEY_TO_WALL - DISTANCE_BUMPER_WIDTH, 0.55 );
		
		AutonDriveInches( AUTON_DIST, 0.45 );
				
		
		ballCannon->SetAngle(AUTON_BALL_ANGLE);
		// need to manually update it, no loop
		ballCannon->AutoUpdateMoving();
		
		
		ballCollector->FireAuto();
		
}

void SimpleAuton3Pointer(void){
		
		// This is the simplest autonomous routine without multithreading
		// TODO: Run these concurrently using thread to save time, if needed
		
		ballCollector->On();
		ballCannon->SetPower(.50);
		ballCannon->Calibrate();
		
		
		
		while (ballCannon->calibrating)
			ballCannon->Update();
		
		
		Wait(5.0);
		
		//AutonDriveInches( DISTANCE_FROM_KEY_TO_WALL - DISTANCE_BUMPER_WIDTH, 0.55 );
		
		//AutonDriveInches( 100.0, 0.45 );
				
		
		ballCannon->SetAngle(AUTON_BALL_ANGLE);
		// need to manually update it, no loop
		while (ballCannon->moving)
				ballCannon->Update();
		
		
		ballCollector->FireAuto();
		
}
	
void Autonomous(void){
		
		leroyDrive->SetSafetyEnabled(false);
		
		SimpleAutonOne();
		//SimpleAuton3Pointer();
}

void OperatorModeSetup(void){
		
		DEBUG_PRINT("Normal operator mode. In setup.\n");
		ballCannon->Calibrate();
		
		
}



void OperatorModeControlLoop(void){
		
	TankDriveMe();
	ballCollector->On(); /*  rg */
	
	ballCannon->Update();
	ballCannon->DirectDriveAngle(joystickTwo->GetRawAxis(LEFT_Y_AXIS));
	
	if ( joystickTwo->GetRawButton(A_BUTTON) ){
		ballCannon->SetPower(0.25);
		ballCollector->FireAuto();
	} 
	else if ( joystickTwo->GetRawButton(B_BUTTON) ){
		ballCannon->SetPower(0.50);
		ballCollector->FireAuto();
	} 
	else if ( joystickTwo->GetRawButton(Y_BUTTON) ){
		ballCannon->SetPower(1.0);
		ballCollector->FireAuto();
	} 
	else {
		ballCollector->Hold();
	}
	
	if ( joystickOne->GetRawButton(B_BUTTON) ){
			bridgeSlapper->Deploy();
		} 
		else if ( joystickOne->GetRawButton(X_BUTTON) ){
			bridgeSlapper->Undeploy();
		} 
		
	dsLCD->Printf(DriverStationLCD:: kUser_Line4, 1, "BC: [%1.2f]", ballCannon->GetCurrentAngle());
	dsLCD->UpdateLCD();
	
}
	

void OperatorControl(void){
		
		DEBUG_PRINT("Starting operator controlled mode 1.\n");
		
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
		
		//testButtons(joystickOne);
		//int joy = testSubModeToggleButton->State() + 1;
		//	printf("Stick %d is at %f\n", joy, joystickOne->GetRawAxis(joy));
		
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
	
	
	
	testMode = kRawIO;
	subTestMode = 0;
	startTest = true;
	
};

void TestModeControlLoop(void){
	
	dsLCD->UpdateLCD();
	
		
	int mode = testModeToggleButton->State();
	
	if (testMode!=mode || startTest){
		printf("Switching to test mode %d.", testMode);
		testMode = (TestModes)mode;
		startTest = false;
	}
	
	switch (testMode){
	
	
	
	case kRawIO:
	{
		dsLCD->Printf(DriverStationLCD:: kUser_Line1, 1, "LeRoy, here. I am in RAW I/O test mode.");
		printf("DIN A: %d  DIN B: %d\n", dinA->Get(), dinB->Get());
		int relay = SPIKE_BRIDGE_ACTUATOR;
		if (testRelay==NULL){
			
			testRelay = new Relay(relay, Relay::kBothDirections);
		}
		
		// DEPLOY
		if (joystickOne->GetRawButton(A_BUTTON)){
					DEBUG_PRINT("Turning on relay %d!\n", relay);
					
					while (!dinA->Get() || joystickOne->GetRawButton(Y_BUTTON))
						testRelay->Set(Relay::kReverse);
					
					testRelay->Set(Relay::kOff);
					return;
				}
		
		// UNDEPLOY
		if (joystickOne->GetRawButton(B_BUTTON)){
							DEBUG_PRINT("Turning on relay %d!\n", relay);
							
							while (!dinB->Get() || joystickOne->GetRawButton(Y_BUTTON))
								testRelay->Set(Relay::kForward);
							
							testRelay->Set(Relay::kOff);
							return;
						}
		
		if (joystickOne->GetRawButton(B_BUTTON)){
							DEBUG_PRINT("Reverse relay!\n");
							testRelay->Set(Relay::kForward);
							return;
						}
		
		if (joystickOne->GetRawButton(X_BUTTON)){
									DEBUG_PRINT("Off relay!\n");
									testRelay->Set(Relay::kOff);
									return;
								}
		
	}
	break;
	
	/*
		 *  BallCollectorsystem Test Code 
		 * 
		 */
	
	case kBallCollector:
	{
		dsLCD->Printf(DriverStationLCD:: kUser_Line1, 1, "LeRoy, here. I am in BALL COLLECTOR test mode.");
		if (ballCollector==NULL){
			ballCollector = new BallCollectionSystem();
		}
		
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
					
					//ballCollector->Fire();
					
					ballCollector->Fire();;
					
					return;
				}
		
		if (joystickOne->GetRawButton(START_BUTTON)){
					DEBUG_PRINT("Help: A=ON, B=Off, C=Reverse, Y=Fire\n");
					ballCollector->On();
					ballCollector->Fire();
					return;
				}
		
		ballCollector->Off(); // No button, do nothing
		break;
	};
	
	case kBallCannon:
	{
		dsLCD->Printf(DriverStationLCD:: kUser_Line1, 1, "LeRoy, here. I am in BALL CANNON test mode.");
		ballCollector->Off();
		
		
		
		if (ballCannon==NULL){
			ballCannon = new BallCannon();
			printf("Brought up ball cannon\n");
					}
		
		// Must call update method so Fire() will work
		ballCannon->Update();
		
		float val = joystickOne->GetRawAxis(LEFT_X_AXIS);
		//DEBUG_PRINT("Raw axis %f\n", val);
		ballCannon->DirectDriveAngle(val);
		
		// Button A turns on call
		if (joystickOne->GetRawButton(A_BUTTON)){
			DEBUG_PRINT("Starting calibration\n");
			ballCannon->Calibrate();
			return;
		}
		
		// Button B is off
		if (joystickOne->GetRawButton(B_BUTTON)){
			//DEBUG_PRINT("Turning off ball collector!\n");
			//ballCollector->Off();
			return;
		}
		
		if (joystickOne->GetRawButton(X_BUTTON)){
			//DEBUG_PRINT("Reversing ball collector!\n");
			ballCannon->CancelCal();
			return;
		}
		
		if (joystickOne->GetRawButton(Y_BUTTON)){
					//DEBUG_PRINT("Driving to 0.\n");
					
					
					ballCannon->SetAngle(45.0);
					
					
					return;
				}
		
		if (joystickOne->GetRawButton(START_BUTTON)){
					DEBUG_PRINT("Help: A=ON, B=Off, C=Reverse, Y=Fire\n");
					ballCollector->On();
					ballCollector->Fire();
					return;
				}
		
		ballCollector->Off(); // No button, do nothing
		
		dsLCD->Printf(DriverStationLCD:: kUser_Line3, 1, "Angle: %1.2f", ballCannon->GetCurrentAngle());
				
		break;
	};
	
	case kBridgeActuator:
	{
		dsLCD->Printf(DriverStationLCD:: kUser_Line1, 1, "LeRoy, here. I am in BRIDGE SLAPPER test mode.");
		DEBUG_PRINT("Bridge splapper\n");
		ballCollector->Off();
		ballCannon->SetPower(0);
		
		
		
		if (bridgeSlapper==NULL){
			bridgeSlapper = new BridgeActuator();
			printf("Brought up slapper\n");
		}
		
		// Must call update method so Fire() will work
		bridgeSlapper->Update();
		
		
		
		// Button A deployed
		if (joystickOne->GetRawButton(A_BUTTON)){
			DEBUG_PRINT("Deploy bridge\n");
			bridgeSlapper->Deploy();
			return;
		}
		
		
		if (joystickOne->GetRawButton(X_BUTTON)){
			DEBUG_PRINT("Undeploy bridge splapper\n");
			bridgeSlapper->Undeploy();
			return;
		}
		
	
		break;
	};
	
	case kDriveTrain:
		{
			dsLCD->Printf(DriverStationLCD:: kUser_Line1, 1, "LeRoy, here. I am in BASIC DRIVETRAIN test mode.");
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
					
			leroyDrive->TankDrive(joystickOne->GetRawAxis(LEFT_Y_AXIS)*0.55, joystickOne->GetRawAxis(RIGHT_Y_AXIS)*0.55);
			DEBUG_PRINT("Encoder dist: %1.2lf raw data %d\n", driveEncoder->GetDistance(), driveEncoder->GetRaw());
			dsLCD->Printf(DriverStationLCD:: kUser_Line2, 5, "Encoder dist: %1.2lf raw data %d\n", driveEncoder->GetDistance(), driveEncoder->GetRaw());
			
			
		}
		break;
	default:
		break;
		
	
	};
	
};

#endif // TEST_CODE
		
};


START_ROBOT_CLASS(RobotDemo);
