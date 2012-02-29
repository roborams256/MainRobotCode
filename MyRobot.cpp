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

	DriverStationLCD *dsLCD;
	RobotDrive *leroyDrive; // robot drive system
	
	TriggerWheel *tweel;
	BallCannon *ballCannon;
	BallCollectionSystem *ballCollector;
	
	Jaguar *angleJ;
	
	Joystick *joystickOne;
	Joystick *joystickTwo;
	Relay *relay;
	
	ToggleButton *ballCollectorToggle;
	
	ParameterStorage *pstore;
	
	AnalogChannel *aIn;
	DigitalInput *dIn;
	
	Gyro *gyro1;
	Gyro *gyro2;
	

	ShotQueue *shotQueue;
	
	OneShotButton *lbButton;
	OneShotButton *rbButton;
	
	
	

public:
	
	RobotDemo(void){
		
	};
	
void RobotInit(){
	
	DEBUG_PRINT("In RobotInit:: \n");
	
	//leroyDrive = new RobotDrive(PWM_LEFT_DRIVE, PWM_RIGHT_DRIVE);
	
	//leroyDrive->SetExpiration(300);
	
	GetWatchdog().SetEnabled(false);
	
	joystickOne = new Joystick(1);
	joystickTwo = new Joystick(2);
	
	dsLCD = DriverStationLCD::GetInstance();
	
	pstore = new ParameterStorage();
	
	ballCannon = new BallCannon();
	ballCollector = new BallCollectionSystem();
	
	gyro1 = new Gyro(1);
	gyro2 = new Gyro(2);
	
	gyro1->Reset();
	gyro2->Reset();
	
	shotQueue = new ShotQueue(ballCollector, ballCannon);
	
	lbButton = new OneShotButton(joystickOne, LB_BUTTON);
	rbButton = new OneShotButton(joystickOne, RB_BUTTON);
	
	ballCannon->Calibrate();	
			
	
}
	

void Disable(){
	
	DEBUG_PRINT("Disable::\n");
}
	
void Autonomous(void){
		
	DEBUG_PRINT("In Autonomous::\n");
	
	while (IsAutonomous()){
		
		
		UpdateAll();
	}
	
	DEBUG_PRINT("AUTON Over\n");
}


	

void OperatorControl(void){
		
		DEBUG_PRINT("Starting operator controlled mode.\n");
		
		// TODO: What do we need to do with this?
		//leroyDrive->SetSafetyEnabled(false);
		
		
		//leroyDrive->SetSafetyEnabled(true);
		while (IsOperatorControl())
		{
			
			
			if (joystickOne->GetRawButton(A_BUTTON)){
				DEBUG_PRINT("A pressed\n");
				tweel->FireSemiAuto();
				
			}
			
			if (joystickOne->GetRawButton(Y_BUTTON)){
							DEBUG_PRINT("Calibrate pressed\n");
							ballCannon->Calibrate();
							
				}
			
			if (joystickOne->GetRawButton(X_BUTTON)){
				DEBUG_PRINT("Calibrate pressed\n");
				ballCannon->SetAngle(45);
										
			}
			
			if (joystickOne->GetRawButton(B_BUTTON)){
							DEBUG_PRINT("Calibrate pressed\n");
							ballCannon->SetAngle(0);
													
						}
			
			if (lbButton->HasFired()){
				shotQueue->AddShot(new Shot(0.75, 25.0));
			}
			
			if (rbButton->HasFired()){
				shotQueue->AddShot(new Shot(0.25, 55.0));
			}
			
			
			float speed = joystickOne->GetRawAxis(TRIGGER_AXIS);
			dsLCD->Printf(DriverStationLCD::kUser_Line1, 1, "Speed %1.2f",speed);
			dsLCD->Printf(DriverStationLCD::kUser_Line2, 1, "A: %1.2f  D: %d", 
					ballCannon->GetVoltage(), ballCannon->GetLimitSensor());
			dsLCD->Printf(DriverStationLCD::kUser_Line3, 1, "Angle: %1.2f", 
								ballCannon->GetCurrentAngle());
			dsLCD->Printf(DriverStationLCD::kUser_Line4, 1, "G1: %1.2f  G2: %1.2f", 
											gyro1->GetAngle(), gyro2->GetAngle());
						
			
			dsLCD->UpdateLCD();
			
			ballCannon->DirectDriveAngle(speed);
			
			
			
			UpdateAll();
			Wait(0.01);				// wait for a motor update time
		} // end of operator control loop
		
		
		DEBUG_PRINT("Leaving operator control\n");
}


void UpdateAll(){
	
	ballCannon->Update();
	ballCollector->Update();
	shotQueue->Update();
	
}

		
};


START_ROBOT_CLASS(RobotDemo);
