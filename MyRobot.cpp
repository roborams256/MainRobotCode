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


class RobotDemo : public SimpleRobot
{
	
	
	/* Different driving modes are enumerated below
	 */
	
	enum DriverStates {
		kDSFreeDriveFull = 0,
		kDSFreeDriveSlow,
		kDSFreeDriveFiltered,
		kDSBridgeTraverse,
		kDSBridgeBalance,
		kDSGyroDrive,
		kDSActiveBreak
	};
	
	// This flag is used to distinguish if the robot is in a semi-auto mode like bridge and balance
	bool  isDrivingTeleOp; 
	
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
	
	DriverStates 	driverState, savedDriverState;
	ShooterStates 	shooterState;

	DriverStationLCD *dsLCD;
	RobotDrive *robotDrive; // robot drive system
	
	BallCannon *ballCannon;
	BallCollectionSystem *ballCollector;
	BridgeActuator *bridgeActuator;
	
	bool bridgeDeployed;
	
	// For testing
	Jaguar *spareJag;
	Relay *relay;
	
	Joystick *gamepadOne;
	Joystick *gamepadTwo;
	
	// Buttons that will switch between modes when pressed
	
	ToggleButton *driveModeToggle;
	ToggleButton *shooterModeToggle;
	
	// Buttons that will perform a single action when pressed
	
	// SHOOTER BUTTONS
	OneShotButton *shooterA;
	OneShotButton *shooterB;
	OneShotButton *shooterX;
	OneShotButton *shooterY;
	OneShotButton *shooterStart;
	OneShotButton *shooterRB;
	
	ToggleButton *ballCollectorToggle;
	int lastBallCollectorState;
	
	
	// DRIVER BUTTONS
	OneShotButton *driverA;
	OneShotButton *driverB;
	OneShotButton *driverX;
	OneShotButton *driverY;
	OneShotButton *driverRB;
	OneShotButton *driverLB;
	OneShotButton *driverStart;
	OneShotButton *activeBreakButton;
	
	float currentShooterSpeed;
	float currentShooterAngle;
	
		
	
	ParameterStorage *pstore;
	
	AnalogChannel *aIn;
	DigitalInput *dIn;
	
	Gyro *directionGyro;
	Gyro *tiltGyro;
	
	Encoder *driveEncoder;
	
	bool flushingCollector;

	ShotQueue *shotQueue;
	Shot *shotArrayOffense[4];
	Shot *shotArrayDefense[4];
	int lastShotButtonPushed;
	
	FlexibleScaler *leftDriveFilter;
	FlexibleScaler *rightDriveFilter;
	
	// The bridge traversal is so complex, it feels like it needs to be its own object
	// but for now, keep it here.
	enum BridgeTraversalStates {
		kBTSDeployingBridgeActuator,
		kBTSDrivingFrontWheelsOn,
		kBTSDrivingToCenter,
		kBTSRollingOff
	};
	
	DigitalInput *autonMode;
	DigitalInput *autonMode2;
	
	BridgeTraversalStates btState;
	

public:
	
	RobotDemo(void){
		
		//printf("In Robot Demo constructor.\n");
		// RobotDrive class is a built in class for controlling a basic drivetrain like ours
		robotDrive = new RobotDrive(PWM_LEFT_DRIVE, PWM_RIGHT_DRIVE);	
		robotDrive->SetExpiration(300);
		robotDrive->SetSafetyEnabled(false);
		
		
		// Don't use the User watchdog timer (WDT). System WDT is still used and cannot be 
		// interacted with.
		GetWatchdog().SetEnabled(false);
		
		bridgeDeployed = false;
		
		// set up the gamepads
		gamepadOne = new Joystick(1);
		gamepadTwo = new Joystick(2);
		
		shooterA = new OneShotButton(gamepadTwo, A_BUTTON);
		shooterB = new OneShotButton(gamepadTwo, B_BUTTON);
		shooterX = new OneShotButton(gamepadTwo, X_BUTTON);
		shooterY = new OneShotButton(gamepadTwo, Y_BUTTON);
		shooterStart = new OneShotButton(gamepadTwo, START_BUTTON);
		shooterRB = new OneShotButton(gamepadTwo, RB_BUTTON);
		
		ballCollectorToggle = new ToggleButton(gamepadTwo, LB_BUTTON, 3);
		lastBallCollectorState = ballCollectorToggle->State();
		
		
		// Driver button switches between the three teleop drive modes. Will ultimately be two.
		driverRB = new OneShotButton( gamepadOne, RB_BUTTON  );
		
		// LB toggles in and out of gyro drive mode
		driverLB = new OneShotButton( gamepadOne, LB_BUTTON  );
		
		driverY = new OneShotButton( gamepadOne, Y_BUTTON );
		driverB = new OneShotButton( gamepadOne, B_BUTTON );
		
		driverStart = new OneShotButton( gamepadOne, START_BUTTON );
		activeBreakButton = new OneShotButton( gamepadOne, LB_BUTTON);
		
		
				
		
		// The DriverStationLCD class lets us write to the driver station screen.
		dsLCD = DriverStationLCD::GetInstance();
		
		// ParameterStorage mimics an ObjectiveC or Java Dictionary but is non-volatile.
		// The class writes to the on-board Flash.
		pstore = new ParameterStorage();
		
		
		// BallCannon class controls the tilt and speed of the ball launch wheels
		ballCannon = new BallCannon();
	
		// BallCollectionSystem controls the Sweeper, the Elevator and the Trigger Wheel
		ballCollector = new BallCollectionSystem();
		
		// BridgeActuator actuates the bridge, yo
		
		bridgeActuator = new BridgeActuator();
		
		// Two Gyros are used. The directionGyro is mounted horizontally and measures direction.
		// The tiltGyro is mounted vertically and is used to measure front/back balance.
		directionGyro = new Gyro(1);
		tiltGyro = new Gyro(2);
		
		// Set the current angle for both to 0 degrees
		//directionGyro->Reset();
		//tiltGyro->Reset();
		
		
		// Drive encoder
		
		driveEncoder = new Encoder(DIO_ENCODER_A, DIO_ENCODER_B, true, CounterBase::k4X);
				
		driveEncoder->Reset();
		driveEncoder->SetDistancePerPulse(INCHES_PER_TICK);
		driveEncoder->Start();
		
		// The ShotQueue queues up Shot objects and then performs the sequencing to make a shot.
		// A Shot is an angle and a speed. The Shot queue makes sure the angle and speed are set before
		// releasing a shot.
		shotQueue = new ShotQueue(ballCollector, ballCannon);
		
		
		ballCannon->Calibrate();	
		
		
		// This is the low pass filter scaler. Number of terms is set in Constants.h
		// More terms = smoother but slower.
		// The lpFilter object is used to smooth out driver controls for smoother operation of the robot.
		leftDriveFilter = new FlexibleScaler(FlexibleScaler::kLPFilter, 1.0);
		rightDriveFilter = new FlexibleScaler(FlexibleScaler::kLPFilter, 1.0);
		
		LoadPresetShots();
		
		
		lastShotButtonPushed = -1; // illegal state to start
		
		
		// Used for testing
		//pareJag = new Jaguar(8);
		//dIn = new DigitalInput(14);
		
		autonMode = new DigitalInput(AUTON_SWITCH_INPUT);
		autonMode2 = new DigitalInput(AUTON_SWITCH_2);
		
		DEBUG_PRINT("Leavng Robot Demo constructor.\n");
		
		
	};
	

	
void RobotInit(){
	
	//printf("In RobotInit:: \n");
	
	directionGyro->Reset();
	

	
}
	

void Disable(){
	
	DEBUG_PRINT("Disable::\n");
}
	
void Autonomous(void){
	
	// The Autonomous mode code is not in Version 2.0 yet.
		
	DEBUG_PRINT("In Autonomous::\n");
	
	int bit0 = autonMode->Get();
	int bit1 = autonMode2->Get();
	
	if 		(bit0 && bit1)
		AutonFreeThrowOnly();
	else if (!bit0 && bit1)
		AntiBellarmineMode();
	else if (bit0 && !bit1)
		AutonTeamBridgeMode();
	else if (!bit0 && !bit1)
		AntiBellarmineMode2();
	
	DEBUG_PRINT("AUTON Over\n");
}


	

void OperatorControl(void){
		
		//printf("Starting operator controlled mode.\n");
		
		//bridgeActuator->Undeploy();
		
		ballCannon->Calibrate();
		ballCollector->On();
		
		shotQueue->PurgeQueue();
		flushingCollector = false;
		
		// start in full drive mode
		SetDriverState(kDSFreeDriveSlow);
		
		
		isDrivingTeleOp = true;
		DEBUG_PRINT("Starting operator controlled mode1.\n");
		
		// We wake up on the offense side of the field
		SetShooterState(kSSDefense);
				
		// This is the main operation loop in tele operated mode
		
		
		while (IsOperatorControl())
		{
			
			DEBUG_PRINT("===== Distance %f ", driveEncoder->GetDistance());
			DEBUG_PRINT("G1 %1.2f G2 %1.2f\n", directionGyro->GetAngle(), tiltGyro->GetAngle());
			
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
		bridgeActuator->Update();
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
		directionGyro->Reset();
		break;
	case kDSActiveBreak:
			dsLCD->Printf(DriverStationLCD::kUser_Line1, 1, "Active Break");
			break;
	case kDSFreeDriveFiltered:
		dsLCD->Printf(DriverStationLCD::kUser_Line1, 1, "Fast Filter Drive Mode");
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
	
	// Check the relevant driver buttons
	
	// Manually process bridge actuator. Trigger buttons.
	
	if (gamepadOne->GetRawAxis(TRIGGER_AXIS)>0.2){
			bridgeActuator->Forward();
			bridgeActuator->Reset();
		}
		else if (gamepadOne->GetRawAxis(TRIGGER_AXIS)<-0.2){
			bridgeActuator->BackwardUnsafe();
			bridgeActuator->Reset();
		}
		else
			bridgeActuator->OffIfNotDeploying();  // Can't just call OFF cuz it stops a deploy cold
	
	// START button toggles bridge full up, full down
	/*if (driverStart->HasFired()){
		bridgeActuator->Toggle();
	}*/
	
	// RB controls the tele-op drive mode. Later this will be assigned to a ToggleButton
	if (driverRB->HasFired()){
		
		// Change: only alternates between Slow and Filtered
		switch (driverState){
		case kDSFreeDriveFull:
			SetDriverState(kDSFreeDriveSlow);
			break;
		case kDSFreeDriveSlow:
			SetDriverState(kDSFreeDriveFull);
			break;
		case kDSFreeDriveFiltered:
			SetDriverState(kDSFreeDriveSlow);
			break;
		default:
			break;
		}		
	}
	
	/*
	// LB toggles in and out of Gyro Drive
	if (driverLB->HasFired()){
		// if not in gyro, go
		if ( driverState!=kDSGyroDrive ){
			savedDriverState = driverState; // so we can restore on exit from Gyro
			// current angle is te reference angle
			directionGyro->Reset();
			SetDriverState(kDSGyroDrive);
		} 
		else
		{
			// we are exiting gyro drive
			SetDriverState(savedDriverState); // just restore the old one
		}
		
	}
	*/
	
	if (activeBreakButton->HasFired()){
		if ( driverState!=kDSActiveBreak ){
					savedDriverState = driverState; // so we can restore on exit from Gyro
					// current angle is te reference angle
					driveEncoder->Reset();
					directionGyro->Reset();
					SetDriverState(kDSActiveBreak);
				} 
				else
				{
					// we are exiting gyro drive
					SetDriverState(kDSFreeDriveSlow); // just restore the old one
				}
	}
		
	
	if (driverB->HasFired())
		SetDriverState(kDSBridgeTraverse);

	// Process drive controls
	
	switch (driverState){
			
		// TODO: add safety measures based on tiltGyro...correct based on tilt if needed.
	
		case kDSFreeDriveFull:
			robotDrive->TankDrive(gamepadOne->GetRawAxis(LEFT_Y_AXIS), 
					gamepadOne->GetRawAxis(RIGHT_Y_AXIS));
			break;
				
		case kDSFreeDriveSlow:
			//TODO put the scaler in Constants
			robotDrive->TankDrive(gamepadOne->GetRawAxis(LEFT_Y_AXIS)* SLOW_DRIVE_SCALER, 
								gamepadOne->GetRawAxis(RIGHT_Y_AXIS)* SLOW_DRIVE_SCALER);
			break;
				
		case kDSFreeDriveFiltered:
				
			float ldrive = leftDriveFilter->Scale(gamepadOne->GetRawAxis(LEFT_Y_AXIS));
			float rdrive = rightDriveFilter->Scale(gamepadOne->GetRawAxis(RIGHT_Y_AXIS));
						
			robotDrive->TankDrive(ldrive, rdrive);		
			break;
				
		case kDSGyroDrive:
				
			// the Drive method on RobotBase takes a speed and a curve angle. We get the speed from the right Y
			// joystick (the left is ignored). The curve is the current angle away from zero divided by 10. 
			// When gyro drive starts, the angle is reset to zero (see above). Then, any measured angle that
			// isn't 0 degrees is an ERROR (the robot is not going straight). So by feeding a fraction of this
			// error into the curve causes the robot to curve back to try to make this angle 0 degrees again
			// and therefore go straight. Like magic :).
			
			//printf("Gyro drive: %1.2f\n", directionGyro->GetAngle());
			robotDrive->Drive(gamepadOne->GetRawAxis(RIGHT_Y_AXIS), directionGyro->GetAngle()/10.0);
			
			break;
			
		case kDSActiveBreak:
						
					// the Drive method on RobotBase takes a speed and a curve angle. We get the speed from the right Y
					// joystick (the left is ignored). The curve is the current angle away from zero divided by 10. 
					// When gyro drive starts, the angle is reset to zero (see above). Then, any measured angle that
					// isn't 0 degrees is an ERROR (the robot is not going straight). So by feeding a fraction of this
					// error into the curve causes the robot to curve back to try to make this angle 0 degrees again
					// and therefore go straight. Like magic :).
					
					//printf("Gyro drive: %1.2f\n", directionGyro->GetAngle());
					robotDrive->Drive(-driveEncoder->GetDistance()/5.0, directionGyro->GetAngle()/5.0);
					
					
					// if the user touches the sticks, stop breaking
					if ( fabs( gamepadOne->GetRawAxis(RIGHT_Y_AXIS) )> 0.5 || 
							fabs( gamepadOne->GetRawAxis(LEFT_Y_AXIS) )> 0.5 ) {
						SetDriverState(kDSFreeDriveSlow);
					}
					break;
			
		case kDSBridgeTraverse:
		
			// this is a big chunk of code, so move it to a method for better readibility
			BridgeTraverse();
			
			break;
		
		case kDSBridgeBalance:
			
			// In bridge balance mode, we'll be rocking back and forth using both gyros.
			// But to stay in bridge balance mode, the Y button must be held down as a safety measure.
			
			if (!gamepadOne->GetRawButton(Y_BUTTON)){
				// user has let go of Y button, so abandon balance mode
				SetDriverState(kDSFreeDriveSlow); // is this what we want on exit?
			}
			else
			{
				// balance...
				// Use the Drive method again. Speed/Direction is a fraction of the tilt angle.
				// Perfect balance and the angle should be 0 so drive speed will be 0. If we're
				// at a 30 degree angle then the drive will be 1.0 (or -1.0) which is max. This means
				// the closer you are to a zero angle, the gentler the adjustment. Direction gyro drive
				// works the same as Gyro drive.
				
				//robotDrive->Drive( tiltGyro->GetAngle()/30.0, directionGyro->GetAngle()/10.0 );
			}
			
		
			break;

				 
		default:
			break;
		
		
		} 
	
	
	
	
	
	}

void BridgeTraverse(){
	
	// This is a self contained loop that sort of replaces the main loop while it runs 
	
	//dsLCD->Printf(DriverStationLCD:: kUser_Line1, 1, "Bridge Auto Mode");
	//dsLCD->UpdateLCD();
	
	
	directionGyro->Reset();
	
	// back up 5 inches
	AutonDriveInchesGyro(4.0, 0.45);
	
	bridgeActuator->Deploy();
		
	// We need to manually tick the updates here so the bridge can deploy
	while (bridgeActuator->GetRawVoltage()> 1.0){
		UpdateAll();
		Wait(0.01);
		if (gamepadOne->GetRawButton(X_BUTTON))
			break;
	}
		
	AutonDriveInchesGyro(-2.0, 0.45);
	
	while (!bridgeActuator->isDown){
		if (gamepadOne->GetRawButton(X_BUTTON))
					break;
		UpdateAll();
			
	}
		
	Wait(0.25);
	
	
	
	// The autondrive method calls UpdateAll() for us so the slapper should undeploy during drive
	AutonDriveInchesGyro(-24.0, 0.65);
	
	bridgeActuator->Undeploy();
	AutonDriveInchesGyro(-15.0, 0.85);
			
			// 60" below is perfect for traverse
	//AutonDriveInchesGyro(-25.0, 0.45);
		
	// roll off...
	SetDriverState(kDSFreeDriveSlow);
	
}
	
void ProcessShooterControls(){
	
	
	// check for reset START button
	
	if (gamepadTwo->GetRawButton(START_BUTTON)) {
		shotQueue->PurgeQueue();
		ballCannon->SetAngle(45.0);
		SetShooterState(kSSDefense);
	}
	
	if (gamepadTwo->GetRawButton(BACK_BUTTON)) {
		shotQueue->PurgeQueue();
		ballCannon->SetAngle(45.0);
		SetShooterState(kSSOffense);
	}
	
	// Check for mode change
	
	if  (shooterRB->HasFired()){
		switch (shooterState){
		case kSSOffense:
			SetShooterState(kSSDefense);
			break;
		case kSSDefense:
			SetShooterState(kSSOffense);
			break;
		default:
			SetShooterState(kSSDefense);
		}
	}
	
	// Shooter adjustments
	
	currentShooterSpeed = ballCannon->GetSpeed();
	currentShooterAngle = ballCannon->GetCurrentAngle();
	
	// Speed is D-Pad
	float dpad = gamepadTwo->GetRawAxis(DPAD_X_AXIS);
	if (dpad>0){
		currentShooterSpeed = currentShooterSpeed+ currentShooterSpeed*0.01;
		if (currentShooterSpeed > 1.0)
			currentShooterSpeed = 1.0;
		}
	else if (dpad<0){
		currentShooterSpeed = currentShooterSpeed - currentShooterSpeed*0.01;
				if (currentShooterSpeed < 0)
					currentShooterSpeed = 0;
		}		

	// Apply any changes from D-Pad, if any
	ballCannon->SetSpeed(currentShooterSpeed);

	// Adjust cannon angle with trigger buttons
	ballCannon->DirectDriveAngle(gamepadTwo->GetRawAxis(TRIGGER_AXIS));
	
	
	// check for flushing collector
	
	int bcState = ballCollectorToggle->State();
	if (bcState!=lastBallCollectorState){
		// Ball collector button state changed so let's update
		printf("BC State: %d\n", bcState);
		switch (bcState){
			
			case 1:
				ballCollector->On();
				break;
			case 2:
				ballCollector->Off();
				break;
			case 3:
				ballCollector->Reverse();
				break;
		}
		lastBallCollectorState = bcState;
	}
	

	

	
	// First, see if any shooter buttons pushed
	switch (shooterState){
	
	case kSSDefense: // no discrete defense mode for now...
	
		if (shooterA->HasFired()){
			// A button is only programmed when Z button interlock is pressed
			if (gamepadTwo->GetRawButton(LEFTZ_BUTTON)){
				Shot *newA = new Shot(currentShooterSpeed, currentShooterAngle);
				shotArrayDefense[0] = newA;
				pstore->SaveParameterAsFloat("defenseASpeed", currentShooterSpeed);
				pstore->SaveParameterAsFloat("defenseAAngle", currentShooterAngle);					
			}			
			
			shotQueue->AddShot(shotArrayDefense[0]);
			lastShotButtonPushed = 0; // 0 = A button. Shoulda used an enum...
		}
		
		else if (shooterB->HasFired()){
			
			if (gamepadTwo->GetRawButton(LEFTZ_BUTTON)){
				Shot *newA = new Shot(currentShooterSpeed, currentShooterAngle);
				shotArrayDefense[1] = newA;
				pstore->SaveParameterAsFloat("defenseBSpeed", currentShooterSpeed);
				pstore->SaveParameterAsFloat("defenseBAngle", currentShooterAngle);
			}
								
			DEBUG_PRINT("DRIVER B FIRED\n");
			shotQueue->AddShot(shotArrayDefense[1]);
			lastShotButtonPushed = 1;
		}
		
		else if (shooterX->HasFired()){
			
			if (gamepadTwo->GetRawButton(LEFTZ_BUTTON)){
				//TODO fix memory leaks
				Shot *newA = new Shot(currentShooterSpeed, currentShooterAngle);
				shotArrayDefense[2] = newA;
				pstore->SaveParameterAsFloat("defenseXSpeed", currentShooterSpeed);
				pstore->SaveParameterAsFloat("defenseXAngle", currentShooterAngle);
			}
			
			shotQueue->AddShot(shotArrayDefense[2]);
			lastShotButtonPushed = 2;
		}
		else if (shooterY->HasFired()){
			
			if (gamepadTwo->GetRawButton(LEFTZ_BUTTON)){
				//TODO fix memory leaks
				Shot *newA = new Shot(currentShooterSpeed, currentShooterAngle);
				shotArrayDefense[3] = newA;
				pstore->SaveParameterAsFloat("defenseYSpeed", currentShooterSpeed);
				pstore->SaveParameterAsFloat("defenseYAngle", currentShooterAngle);
				}
			
			shotQueue->AddShot(shotArrayDefense[3]);
			lastShotButtonPushed = 3;
		}
	
		break;
		
		
		
		
	case kSSOffense:
		
		if (shooterA->HasFired()){
			// A button is only programmed when Z button interlock is pressed
			if (gamepadTwo->GetRawButton(LEFTZ_BUTTON)){
				DEBUG_PRINT("Memorizing A...\n");
				Shot *newA = new Shot(currentShooterSpeed, currentShooterAngle);
				shotArrayOffense[0] = newA;
				pstore->SaveParameterAsFloat("offenseASpeed", currentShooterSpeed);
				pstore->SaveParameterAsFloat("offenseAAngle", currentShooterAngle);					
			}			
			
			DEBUG_PRINT("DRIVER A FIRED\n");
			shotQueue->AddShot(shotArrayOffense[0]);
			lastShotButtonPushed = 0; // 0 = A button. Shoulda used an enum...
		}
		
		else if (shooterB->HasFired()){
			
			if (gamepadTwo->GetRawButton(LEFTZ_BUTTON)){
				DEBUG_PRINT("Memorizing B...\n");
				Shot *newA = new Shot(currentShooterSpeed, currentShooterAngle);
				shotArrayOffense[1] = newA;
				pstore->SaveParameterAsFloat("offenseBSpeed", currentShooterSpeed);
				pstore->SaveParameterAsFloat("offenseBAngle", currentShooterAngle);
			}
								
			DEBUG_PRINT("DRIVER B FIRED\n");
			shotQueue->AddShot(shotArrayOffense[1]);
			lastShotButtonPushed = 1;
		}
		
		else if (shooterX->HasFired()){
			
			if (gamepadTwo->GetRawButton(LEFTZ_BUTTON)){
				//TODO fix memory leaks
				DEBUG_PRINT("Memorizing X...\n");
				Shot *newA = new Shot(currentShooterSpeed, currentShooterAngle);
				shotArrayOffense[2] = newA;
				pstore->SaveParameterAsFloat("offenseXSpeed", currentShooterSpeed);
				pstore->SaveParameterAsFloat("offenseXAngle", currentShooterAngle);
			}
			
			DEBUG_PRINT("DRIVER X FIRED\n");
			shotQueue->AddShot(shotArrayOffense[2]);
			lastShotButtonPushed = 2;
		}
		else if (shooterY->HasFired()){
			
			if (gamepadTwo->GetRawButton(LEFTZ_BUTTON)){
				//TODO fix memory leaks
				DEBUG_PRINT("Memorizing Y...\n");
				Shot *newA = new Shot(currentShooterSpeed, currentShooterAngle);
				shotArrayOffense[3] = newA;
				pstore->SaveParameterAsFloat("offenseYSpeed", currentShooterSpeed);
				pstore->SaveParameterAsFloat("offenseYAngle", currentShooterAngle);
				}
			
			DEBUG_PRINT("DRIVER Y FIRED\n");
			shotQueue->AddShot(shotArrayOffense[3]);
			lastShotButtonPushed = 3;
		}
		
		break;
		
		
	}
	
	

	// update driver's display
	dsLCD->Printf(DriverStationLCD::kUser_Line5, 1, "Ball Angle %1.2f", ballCannon->GetCurrentAngle());
	dsLCD->Printf(DriverStationLCD::kUser_Line6, 1, "Ball Speed %1.2f", ballCannon->GetSpeed());
		
	dsLCD->UpdateLCD();
	
	
}


/********************************************************
 * 
 * Autonomous and Semi-Autonomous Functions
 * 
 ********************************************************/

void AutonDriveInchesGyro(float inches, float speed){
	
		driveEncoder->Reset();
		
		// Neg inches is backwards so drive backwards
		if (inches<0)
			speed = -speed;
		
		// We really only care about the absolute val since direction is controlled by speed
				
		inches = fabs(inches);  
		
		// X Button on driver's controller cancels out in case of hang
		while ( (fabs(driveEncoder->GetDistance()) < inches) && !gamepadOne->GetRawButton(X_BUTTON) ){
			float angle = directionGyro->GetAngle();
			
			// The line below uses the angle as an ERROR from 0 degrees. The larger the error, the more 
			// leroy will turn. Drive(speed, turn) method used below.
			
			robotDrive->Drive(speed, -angle/15.0); 
			
			dsLCD->Printf(DriverStationLCD:: kUser_Line2, 1, "Auton: %1.2f inches", inches-driveEncoder->GetDistance());
			dsLCD->Printf(DriverStationLCD:: kUser_Line3, 1, "Gyro: %1.2f deg", angle);
			dsLCD->UpdateLCD();
			
			// Need to keep everything updated while in any loop
			UpdateAll();
			}
								
		robotDrive->Drive(0.0, 0.0); 	// stop robot

}


void SimpleAutonOne(void){
		
		// This is the simplest autonomous routine
		
		directionGyro->Reset();
		ballCollector->On();
		Wait(1.0);
		
		// Drive to bumper
		AutonDriveInchesGyro( -AUTON_DIST, 0.45 );
		
		// Shoot at middle basket until auton mode is over. Keep shooting in case someone feeds us balls.
		while (IsAutonomous()){
			shotQueue->AddShot(shotArrayOffense[0]);
			UpdateAll();								
		}
		
		// Get rid of any unused shots
		shotQueue->PurgeQueue();
}

void AutonFreeThrowOnly(void){
		
		// This is the simplest autonomous routine
		//directionGyro->Reset();
		ballCollector->On();
		ballCannon->SetAngle(50.0);
		for (int i=0; i<25; i++)
		{
			UpdateAll();
			Wait(0.1);
		}
		
		Wait(0.5);
		
		
		// Y shot on ofense needs to be calibrated
		shotQueue->AddShot(shotArrayOffense[3]);
		shotQueue->AddShot(shotArrayOffense[3]);
		shotQueue->AddShot(shotArrayOffense[3]);
						
		while (IsAutonomous()){
			Wait(0.25);
			UpdateAll();
		}
			
		shotQueue->PurgeQueue();
		
		
}

void AntiBellarmineMode(void){

	directionGyro->Reset();
	
	
	AutonDriveInchesGyro(-ABM_DISTANCE, 0.55); //don't go crazy fast in autonomous. that's DANGEROUS
	
	ballCollector->On();
	
	bridgeActuator->Deploy();
	for (int i=0; i<50; i++)
		{
			UpdateAll();
			Wait(0.1);
		}
	
	// should be reprogrammed with actual calibrated backwards shots
	
	shotQueue->AddShot(new Shot(0.85, 100));
	shotQueue->AddShot(new Shot(0.85, 100));
	shotQueue->AddShot(new Shot(0.85, 100));
	shotQueue->AddShot(new Shot(0.85, 100));
		
							
	while (IsAutonomous()){
		Wait(0.25);
		UpdateAll();
	}
				
	shotQueue->PurgeQueue();
	
	
}

void AntiBellarmineMode2(void){

	directionGyro->Reset();
	
	
	AutonDriveInchesGyro(-ABM_DISTANCE - 6, 0.55); //don't go crazy fast in autonomous. that's DANGEROUS
	
	ballCollector->On();
	
	bridgeActuator->Deploy();
	for (int i=0; i<50; i++)
		{
			UpdateAll();
			Wait(0.1);
		}
	
	// should be reprogrammed with actual calibrated backwards shots
	
	shotQueue->AddShot(new Shot(0.85, 100));
	shotQueue->AddShot(new Shot(0.85, 100));
	shotQueue->AddShot(new Shot(0.85, 100));
	shotQueue->AddShot(new Shot(0.85, 100));
		
							
	while (IsAutonomous()){
		Wait(0.25);
		UpdateAll();
	}
				
	shotQueue->PurgeQueue();
	
	
}

void AutonTeamBridgeMode(void){
	

	// Gyro reset in the Init()
	
	// 'X' on defense is backwards
	
	ballCollector->On();

	for (int i=0; i<50; i++)
		{
			UpdateAll();
			Wait(0.1);
		}

	
	shotQueue->AddShot(shotArrayDefense[2]);
	shotQueue->AddShot(shotArrayDefense[2]);

	for (int i=0; i<50; i++)
		{
			UpdateAll();
			Wait(0.1);
		}

	
	AutonDriveInchesGyro(-TEAM_BRIDGE_DISTANCE, 0.55); //don't go crazy fast in autonomous. that's DANGEROUS
	
	// Turn
	while (directionGyro->GetAngle()<45)
		robotDrive->TankDrive(-0.5, 0.5);
	
	
	bridgeActuator->Deploy();
	
							
	while (IsAutonomous()){
		Wait(0.25);
		UpdateAll();
	}
				
	shotQueue->PurgeQueue();
		
}

void AutonBridgeMode(void){
	
	printf("Auton Bridge Mode\n");
	//directionGyro->Reset();
	//ballCollector->On();
	
	//Fire both shots backwards
	
	ballCannon->SetAngle(50.0);
	for (int i=0; i<25; i++)
	{
		UpdateAll();
		Wait(0.1);
	}

	Wait(0.5);

	Shot *backShot = new Shot(1.0, 100.0); //speed, angle
	shotQueue->AddShot(backShot); //add two backwards shots. these probably wont make it in
	shotQueue->AddShot(backShot);
	
	bool firedBackShots = false;

	while (IsAutonomous()) {
		Wait(0.25);
		UpdateAll();
		
		if (shotQueue->IsEmpty() && firedBackShots == false) //balls fired, we haven't driven yet
		{
			firedBackShots = true;
			AutonDriveInchesGyro(ABM_DISTANCE, 0.5); //don't go crazy fast in autonomous. that's DANGEROUS
		}
		
		if (firedBackShots == true) //we drove, drop the tipper because we're stealing everything from the other team
		{
			bridgeActuator->Forward(); //I think this will work, it should move forward until we hit the bottom limit
			bridgeActuator->OffIfNotDeploying(); //probably redundant
		}
	}

	shotQueue->PurgeQueue(); //get rid of every shot that might happen to be in there
	
	//Drive over to the bridge, then deploy the bridge tipper
	
	/*if (gamepadOne->GetRawAxis(TRIGGER_AXIS)>0.2)
			bridgeActuator->Forward();
		else if (gamepadOne->GetRawAxis(TRIGGER_AXIS)<-0.2)
			bridgeActuator->Backward();
		else
			bridgeActuator->OffIfNotDeploying();*/
	/*
	DCPU-16 Emulator
	assumes that there's string:split & my class library
	
	func
	
	class.DCPU()
	
	function DCPU:__init()
	
		self.PC = 0
		self.SP = 0
		self.reg = 0
	
	end
	*/
	
	
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
