#include "WPILib.h"
//#include <math.h>

// Constants.h has the mappings for PWM, Digital IO Channels, etc.
#include "Constants.h"
#include "Sweeper.h"
#include "ToggleButton.h"
#include "TriggerWheel.h"
#include "TestHarness.h"
#include "AutonomousConstants.h"

#define TEST_MODE

#ifdef TEST_MODE

extern void testButtons(Joystick *joystick);
extern void testAxis(Joystick *joystick);

#endif

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
	Encoder *leftEncoder;
	Encoder *rightEncoder;
	
	DigitalInput *pin1;
	DigitalInput *pin2;
	
	
	
	

public:
	RobotDemo(void){
		leroyDrive = new RobotDrive(PWM_LEFT_DRIVE, PWM_RIGHT_DRIVE);
		
		sweeper = new Sweeper(SPIKE_SWEEPER);
		leroyDrive->SetExpiration(1.1);
		
		joystickOne = new Joystick(1);
		joystickTwo = new Joystick(2);
		
		rightEncoder = new Encoder(1,2,false,CounterBase::k4X);
		leftEncoder = new Encoder(11,12,false,CounterBase::k4X);
		
	
		
		
		//pin1 = new DigitalInput(1);
		//pin2 = new DigitalInput(2);
		

	}
	
	void TankDriveMe(){
		float rdrive = joystickOne->GetRawAxis(LEFT_Y_AXIS) * JOYSTICK_DRIVE_SCALE_FACTOR;
		float ldrive = joystickOne->GetRawAxis(RIGHT_Y_AXIS) * JOYSTICK_DRIVE_SCALE_FACTOR;
		DEBUG_PRINT("Left drive:[%f] | Right drive:[%f]\n", ldrive, rdrive);
		
	    leroyDrive->TankDrive(ldrive, ldrive);
	}

	/**
	 * Drive left & right motors for 2 seconds then stop
	 */
	void Autonomous(void)
	{

		Jaguar *bjl;
		Jaguar *bjr;
		
		rightEncoder->SetDistancePerPulse(4.0/360.0);
		rightEncoder->Reset();
		rightEncoder->Start();
		
		leroyDrive->SetSafetyEnabled(false);
		leroyDrive->Drive(0.3, AUTON_TIMED_CURVE_OFFSET); 	// drive forwards half speed
		
		while (rightEncoder->GetDistance() > -55.2){
			DEBUG_PRINT("Roto %lf\n", rightEncoder->GetDistance());
		}
		
		//    for 2 seconds
		leroyDrive->Drive(0.0, 0.0); 	// stop robot
		
		sweeper->On();
		
		Jaguar *belt = new Jaguar(7);
				
				//Jaguar *angle = new Jaguar(8);
				
				//angle->Set(0.0);
						
		float beltSpeed = -01.0;
		float ballSpeed = -0.30;
				
		belt->Set(beltSpeed);
				
		bjl->Set(ballSpeed);
		bjr->Set(ballSpeed);
		
		
		TriggerWheel *trigger = new TriggerWheel(1, 0.1);
		trigger->fireAuto();
		

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
		//TestHarness *testHarness = new TestHarness(joystickOne);
		ToggleButton *tbutt = new ToggleButton(joystickOne, A_BUTTON, 12);
		
		rightEncoder->SetDistancePerPulse(4.0/360.0);
		rightEncoder->Reset();
		rightEncoder->Start();
		
		leftEncoder->SetDistancePerPulse(4.0/360.0);
		leftEncoder->Reset();
		leftEncoder->Start();

		
		
#endif
		
#ifndef TEST_MODE
		
		//Relay *belt = new Relay(3);
		//belt->Set(Relay::kReverse);
		
		
		
		//Relay *trigger = new Relay(SPIKE_TRIGGER);
		//trigger->Set(Relay::kForward);
		//TriggerWheel *trigger = new TriggerWheel(1, 0.1);
		//ToggleButton *beltbutton = new ToggleButton(joystickOne, 3, 2);
		
		//Jaguar *bjr = new Jaguar(10);
		//Jaguar *bjl = new Jaguar(9);
		//Jaguar *belt = new Jaguar(7);
		
		//Jaguar *angle = new Jaguar(8);
		
		//angle->Set(0.0);
				
		//float beltSpeed = -0.75;
		//float ballSpeed = -0.5;
		
		//belt->Set(beltSpeed);
		
		//bjl->Set(ballSpeed);
		//bjr->Set(ballSpeed);

		
		
#endif
		
		
		
		//leroyDrive->SetSafetyEnabled(true);
		while (IsOperatorControl())
		{
			
#ifndef TEST_MODE
			//sweeper->On();
			// Use left and right joysticks on Gamepad One
			float rdrive = joystickOne->GetRawAxis(LEFT_Y_AXIS) * JOYSTICK_DRIVE_SCALE_FACTOR;
			float ldrive = joystickOne->GetRawAxis(RIGHT_Y_AXIS) * JOYSTICK_DRIVE_SCALE_FACTOR;
			DEBUG_PRINT("Left drive:[%f] | Right drive:[%f]\n", ldrive, rdrive);
			
		    leroyDrive->TankDrive(ldrive, rdrive);
		    
		    
		    /*if (joystickOne->GetRawButton(2) && !trigger->pulsing())
		    	trigger->firePulse();
		    else if (!joystickOne->GetRawButton(2) && trigger->pulsing())
		    	trigger->stopPulse();*/
		    /*
		    if (joystickOne->GetRawButton(2))
		    	trigger->fireSemiAuto();
		    
		    trigger->Update();
		    
		    float jLspeed = -0.5*(1 + joystickTwo->GetRawAxis(GPAD_LEFT_Y_RAW_AXIS));
		    float jRspeed = -0.5*(1 + joystickTwo->GetRawAxis(GPAD_RIGHT_Y_RAW_AXIS));
		    
		    printf("StickL: %1.2f | Belt speed: %1.2f | StickR: %1.2f | Ball Speed: %1.2f\n", jLspeed, beltSpeed, jRspeed, ballSpeed);
		    
		    if (joystickTwo->GetRawButton(1)){
		    	beltSpeed = jLspeed;
		    	belt->Set(beltSpeed);
		    }
		    	
		    if (joystickTwo->GetRawButton(2)){
		    		    	ballSpeed = jRspeed;
		    		    	bjl->Set(ballSpeed);
		    		    	bjr->Set(ballSpeed);
		    		    }
		    
		   
		    float t2 = joystickTwo->GetThrottle();
		    if ( (t2 >0.05) || (t2<-0.05) ) {
		    	angle->Set(t2*0.15);
		    }
		    else
		    	angle->Set(0.0);
		    
		    */
#endif
#ifdef TEST_MODE
		    
		    
		    TankDriveMe();
		    DEBUG_PRINT("L Encoder: %lf  R Encoder:  %lf\n", leftEncoder->GetDistance(),  rightEncoder->GetDistance());
		    //testButtons(joystickOne);
		    
		    
		    
		    //testHarness->Update();
#endif

			Wait(0.005);				// wait for a motor update time
		}
	}
};

START_ROBOT_CLASS(RobotDemo);

