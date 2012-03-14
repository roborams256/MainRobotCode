#include "WPILib.h"
#include "Constants.h"
#include "ToggleButton.h"
#include "AutonomousConstants.h"
#include "FlexibleScaler.h"
#include "BallCollectionSystem.h"
#include "BallCannon.h"
#include "BridgeActuator.h"
#include "ParameterStorage.h"
#include "ShotQueue.h"
#include "Shot.h"
#include "OneShotButton.h"
#include <stdio.h>
#include <queue>
#include <math.h>  // needed for fabs() in auton drive methods

#define TEST_MODE

void testButtons(Joystick *joystick);

void testAxis(Joystick *joystick);

//sublime keymap: { "keys": [""], "command": "" },
//Testing out committing from the other computer

class RobotDemo : public SimpleRobot
{
	
	
	/* Different driving modes are enumerated below
	 */
	
	enum DriverStates {
		kDSFreeDriveFull,
		kDSFreeDriveSlow,
		kDSFreeDriveFiltered,
		kDSBridgeTraverse,
		kDSBridgeBalance,
		kDSGyroDrive
	};
	
	// This flag is used if the robot is in a semi-auto mode like bridge and balance
	bool  isDrivingSemiAutonomous; 
	
	/* Shooter state is Offense/Defense and last target button pressed which corresponds to
	 * baskets and firing positions. We need to track it for recording fine
	 * adjustments.
	 */
	
	enum ShooterButtons {
		kSSNo_Button,
		kSSA_Button,
		kSSB_Button,
		kSSX_Button,
		kSSY_Button		
	};
	
	enum ShooterStates {
		kSSOffense,
		kSSDefense
	};
	
	DriverStates 	driverState;
	ShooterStates 	shooterState;

	DriverStationLCD *dsLCD;
	RobotDrive *robotDrive; // robot drive system
	
	TriggerWheel *tweel;
	BallCannon *ballCannon;
	BallCollectionSystem *ballCollector;
	
	// For testing
	Jaguar *spareJag;
	Relay *relay;
	
	Joystick *gamepadOne;
	Joystick *gamepadTwo;
	
	// Buttons that will switch between modes when pressed
	ToggleButton *ballCollectorToggle;
	ToggleButton *driveModeToggle;
	ToggleButton *shooterModeToggle;
	
	// Buttons that will perform a single action when pressed
	OneShotButton *shooterA;
	OneShotButton *shooterB;
	OneShotButton *shooterX;
	OneShotButton *shooterY;
	OneShotButton *shooterStart;
	
	OneShotButton *driverA;
	OneShotButton *driverB;
	OneShotButton *driverX;
	OneShotButton *driverY;
		
	
	ParameterStorage *pstore;
	
	AnalogChannel *aIn;
	DigitalInput *dIn;
	
	Gyro *directionGyro;
	Gyro *tiltGyro;
	

	ShotQueue *shotQueue;
	Shot *shotArrayOffense[4];
	Shot *shotArrayDefense[4];
	int lastShotButtonPushed;
	
	
	OneShotButton *lbButton;
	OneShotButton *rbButton;
	
	FlexibleScaler *leftDriveFilter;
	FlexibleScaler *rightDriveFilter;
	
	
	

public:
	
	RobotDemo(void){
		
		
		
		
	};
	

	
void RobotInit(){
	
	DEBUG_PRINT("In RobotInit:: \n");
	
	// RobotDrive class is a built in class for controlling a basic drivetrain like ours
	robotDrive = new RobotDrive(PWM_LEFT_DRIVE, PWM_RIGHT_DRIVE);	
	robotDrive->SetExpiration(300);
	robotDrive->SetSafetyEnabled(false);
	
	// Don't use the User watchdog timer (WDT). System WDT is still used and cannot be 
	// interacted with.
	GetWatchdog().SetEnabled(false);
	
	
	// set up the gamepads
	gamepadOne = new Joystick(1);
	gamepadTwo = new Joystick(2);
	
	shooterA = new OneShotButton(gamepadTwo, A_BUTTON);
	shooterB = new OneShotButton(gamepadTwo, B_BUTTON);
	shooterX = new OneShotButton(gamepadTwo, X_BUTTON);
	shooterY = new OneShotButton(gamepadTwo, Y_BUTTON);
	shooterStart = new OneShotButton(gamepadTwo, START_BUTTON);
			
	
	// The DriverStationLCD class lets us write to the driver station screen.
	dsLCD = DriverStationLCD::GetInstance();
	
	// ParameterStorage mimics an ObjectiveC or Java Dictionary but is non-volatile.
	// The class writes to the on-board Flash.
	pstore = new ParameterStorage();
	
	
	// BallCannon class controls the tilt and speed of the ball launch wheels
	ballCannon = new BallCannon();
	
	// BallCollectionSystem controls the Sweeper, the Elevator and the Trigger Wheel
	ballCollector = new BallCollectionSystem();
	
	// Two Gyros are used. The directionGyro is mounted horizontally and measures direction.
	// The tiltGyro is mounted vertically and is used to measure front/back balance.
	directionGyro = new Gyro(1);
	tiltGyro = new Gyro(2);
	
	// Set the current angle for both to 0 degrees
	directionGyro->Reset();
	tiltGyro->Reset();
	
	// The ShotQueue queues up Shot objects and then performs the sequencing to make a shot.
	// A Shot is an angle and a speed. The Shot queue makes sure the angle and speed are set before
	// releasing a shot.
	shotQueue = new ShotQueue(ballCollector, ballCannon);
	
	
	lbButton = new OneShotButton(gamepadOne, LB_BUTTON);
	rbButton = new OneShotButton(gamepadOne, RB_BUTTON);
	
	ballCannon->Calibrate();	
	
	
	// This is the low pass filter scaler. Number of terms is set in Constants.h
	// More terms = smoother but slower.
	// The lpFilter object is used to smooth out driver controls for smoother operation of the robot.
	leftDriveFilter = new FlexibleScaler(FlexibleScaler::kLPFilter, 1.0);
	rightDriveFilter = new FlexibleScaler(FlexibleScaler::kLPFilter, 1.0);
	
	LoadPresetShots();
	lastShotButtonPushed = -1; // illegal state to start
	
	
	// Used for testing
	spareJag = new Jaguar(8);
	
}
	

void Disable(){
	
	DEBUG_PRINT("Disable::\n");
}
	
void Autonomous(void){
	
	// The Autonomous mode code is not in Version 2.0 yet.
		
	DEBUG_PRINT("In Autonomous::\n");
	
	
	
	while (IsAutonomous()){
		
		
		UpdateAll();
	}
	
	DEBUG_PRINT("AUTON Over\n");
}


	

void OperatorControl(void){
		
		DEBUG_PRINT("Starting operator controlled mode.\n");
		
		// start in full drive mode
		SetDriverState(kDSFreeDriveFiltered);
		isDrivingSemiAutonomous = false;
		
		// We wake up on the offense side of the field
		SetShooterState(kSSOffense);
				
		// This is the main operation loop in tele operated mode
		
		while (IsOperatorControl())
		{
			
			ProcessDriverControls();
			ProcessShooterControls();
			
			UpdateAll();
			
			
			// wait for a motor update time
			Wait(0.01);	
			
		} // end of operator control loop
		
		
		DEBUG_PRINT("Leaving operator control\n");
}

void UpdateAll(){
		// Any object that needs to be updated every time thru the run loop gets its Update method
		// called here. This method is called in both teleop and auton mode
	
		ballCannon->Update();
		ballCollector->Update();
		shotQueue->Update();
}


// Sets the Driver State and updates the driver station display
void SetDriverState(DriverStates ds){
	
	driverState = ds;
	
	switch (driverState){
	case kDSFreeDriveFull:
		dsLCD->Printf(DriverStationLCD::kUser_Line1, 1, "Full Drive Mode");
		break;
	case kDSFreeDriveSlow:
		dsLCD->Printf(DriverStationLCD::kUser_Line1, 1, "Slow Drive Mode");
		break;
	case kDSBridgeTraverse:
		dsLCD->Printf(DriverStationLCD::kUser_Line1, 1, "Bridge Traverse");
		break;
	case kDSBridgeBalance:
		dsLCD->Printf(DriverStationLCD::kUser_Line1, 1, "Bridge Balance");
		break;
	case kDSGyroDrive:
		dsLCD->Printf(DriverStationLCD::kUser_Line1, 1, "Gyro Drive");
		break;
	case kDSFreeDriveFiltered:
		dsLCD->Printf(DriverStationLCD::kUser_Line1, 1, "Filter Drive Mode");
		break;
	default:
		dsLCD->Printf(DriverStationLCD::kUser_Line1, 1, "??? Driver Mode");
		break;
	}
	
	dsLCD->UpdateLCD();
}

void SetShooterState(ShooterStates ss){
	
	shooterState = ss;
	
	switch (shooterState){
	case kSSOffense:
		dsLCD->Printf(DriverStationLCD::kUser_Line3, 1, "Offense Mode");
		break;
	case kSSDefense:
		dsLCD->Printf(DriverStationLCD::kUser_Line3, 1, "Defense Mode");
		break;
	default:
		dsLCD->Printf(DriverStationLCD::kUser_Line3, 1, "??? Shooter Mode");
		break;
	}
	
	dsLCD->UpdateLCD();
}


void ProcessDriverControls(){
	
	
	
	switch (driverState){
	
	case kDSFreeDriveFull:
		robotDrive->TankDrive(gamepadOne->GetRawAxis(LEFT_Y_AXIS), gamepadOne->GetRawAxis(RIGHT_Y_AXIS));
		break;
		
	case kDSFreeDriveSlow:
		robotDrive->TankDrive(gamepadOne->GetRawAxis(LEFT_Y_AXIS)*SLOW_DRIVE_SCALER, 
				gamepadOne->GetRawAxis(RIGHT_Y_AXIS)*SLOW_DRIVE_SCALER);
		break;
		
	case kDSFreeDriveFiltered:
		
		float ldrive = leftDriveFilter->Scale(gamepadOne->GetRawAxis(LEFT_Y_AXIS));
		float rdrive = rightDriveFilter->Scale(gamepadOne->GetRawAxis(LEFT_Y_AXIS));
				
		robotDrive->TankDrive(ldrive, rdrive);		
		break;
		
	case kDSBridgeTraverse:
		
		break;
		
	case kDSBridgeBalance:
		
		break;
		
	case kDSGyroDrive:
		
		break;
		 
				
	}
}
	
void ProcessShooterControls(){
	
	// First, see if any shooter buttons pushed
	switch (shooterState){
	
	case kSSOffense:
		
		if (shooterA->HasFired()){
			DEBUG_PRINT("DRIVER A FIRED\n");
			shotQueue->AddShot(shotArrayOffense[0]);
			lastShotButtonPushed = 0; // 0 = A button. Shoulda used an enum...
		}
		else if (shooterB->HasFired()){
			DEBUG_PRINT("DRIVER B FIRED\n");
			shotQueue->AddShot(shotArrayOffense[1]);
			lastShotButtonPushed = 1;
		}
		else if (shooterX->HasFired()){
			DEBUG_PRINT("DRIVER X FIRED\n");
			shotQueue->AddShot(shotArrayOffense[2]);
			lastShotButtonPushed = 2;
		}
		else if (shooterY->HasFired()){
			DEBUG_PRINT("DRIVER Y FIRED\n");
			shotQueue->AddShot(shotArrayOffense[3]);
			lastShotButtonPushed = 3;
		}
		else if (shooterStart->HasFired()){
			ballCannon->Calibrate();
		}
		
		
		break;
		
	case kSSDefense:
		
		break;
		
	}
	
	ballCannon->DirectDriveAngle(gamepadTwo->GetRawAxis(TRIGGER_AXIS));
	dsLCD->Printf(DriverStationLCD::kUser_Line4, 1, "Angle %1.2f", ballCannon->GetCurrentAngle());
	
	
}

// Get the preset shots from ParameterStorage (file on the CRIO "hard disk").
// If this is the first time the ParameterStorage is accessed, we get back 9999 to
// indicate the file doesn't exist. If this were Java, we'd throw and exception :).
	
void LoadPresetShots(){
	
	
	// Get Offense Mode Button settings from hard drive
	 char buttonNames[]="ABXY";
	
	for (int i=0; i<4; i++){
		char paramName[20]; // we used this char array to hold the param name.
		
		// The statement below makes paramName = "offenseASpeed", "offenseBSpeed", etc. each time thru loop
		sprintf(paramName, "offense%cSpeed", buttonNames[i] );
		float speed = pstore->GetParameterAsFloat(paramName);
		if (speed > 900){
			// This means there was no file with a speed, so we'll grab a default and save a file.
			// This code runs the very first time the program is run. EVER. Unless the files are deleted.
			speed = BASELINE_OFFENSE_A_BUTTON_SPEED;
			pstore->SaveParameterAsFloat(paramName, speed);
		}
		
		sprintf(paramName, "offense%cAngle",buttonNames[i] );
		float angle = pstore->GetParameterAsFloat(paramName);
		if (angle > 900){
			// This means there was no file with a speed, so we'll grab a default and save a file.
			// This code runs the very first time the program is run. EVER. Unless the files are deleted.
			angle = BASELINE_OFFENSE_A_BUTTON_ANGLE;
			pstore->SaveParameterAsFloat(paramName, angle);
		}
		
		// create a new Shot from presets
		Shot *aShot = new Shot(speed, angle);
		
		// Save it in the presets array
		shotArrayOffense[i] = aShot;
	}
	
	for (int i=0; i<4; i++){
		char paramName[20]; // we used this char array to hold the param name.
			
		// The statement below makes paramName = "offenseASpeed", "offenseBSpeed", etc. each time thru loop
		sprintf(paramName, "defense%cSpeed", buttonNames[i] );
		float speed = pstore->GetParameterAsFloat(paramName);
		if (speed > 900){
			// This means there was no file with a speed, so we'll grab a default and save a file.
			// This code runs the very first time the program is run. EVER. Unless the files are deleted.
			speed = BASELINE_DEFENSE_A_BUTTON_SPEED;
			pstore->SaveParameterAsFloat(paramName, speed);
		}
			
		sprintf(paramName, "defense%cAngle",buttonNames[i] );
		float angle = pstore->GetParameterAsFloat(paramName);
		if (angle > 900){
			// This means there was no file with a speed, so we'll grab a default and save a file.
			// This code runs the very first time the program is run. EVER. Unless the files are deleted.
			angle = BASELINE_DEFENSE_A_BUTTON_ANGLE;
			pstore->SaveParameterAsFloat(paramName, angle);
		}
			
		// create a new Shot from presets
		Shot *aShot = new Shot(speed, angle);
			
		// Save it in the presets array
		shotArrayDefense[i] = aShot;
	}	
	
}

};


START_ROBOT_CLASS(RobotDemo);
