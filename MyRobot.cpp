#include "MyRobot.h"

RobotDemo::RobotDemo(void){
		
		leroyDrive = new RobotDrive(PWM_LEFT_DRIVE, PWM_RIGHT_DRIVE);
		leroyDrive->SetExpiration(1.1);
		
		joystickOne = new Joystick(1);
		joystickTwo = new Joystick(2);
		
		// The FlexibleScaler object provides 4 different ways of scaling the joystick inputs
		// to provide different "feels" and limits for the joystick. In this one, we are using
		// a parabolic curve (smooth at the bottom, slam hard near the top) and a limit of 75%.
		tankDriveScaler = new FlexibleScaler(FlexibleScaler::kScalePower2AndLinear, 0.75);
		
		driveEncoder = new Encoder(DIO_ENCODER_A, DIO_ENCODER_B, true, CounterBase::k4X);
		
		driveEncoder->Reset();
		driveEncoder->SetDistancePerPulse(INCHES_PER_TICK);
		driveEncoder->Start();
		
		// Initialize the ball collection system
		
		ballCollector = new BallCollectionSystem();
		
};
	
	
	
void RobotDemo::TankDriveMe(){
		
		
		float rdrive = tankDriveScaler->Scale(joystickOne->GetRawAxis(LEFT_Y_AXIS));
		float ldrive = tankDriveScaler->Scale(joystickOne->GetRawAxis(RIGHT_Y_AXIS));
				
		DEBUG_PRINT("Left drive:[%f] | Right drive:[%f]\n", ldrive, rdrive);		
	    
		leroyDrive->TankDrive(ldrive, ldrive);
	    
};
	
void RobotDemo::AutonDriveInches(float inches, float speed){
		
		leroyDrive->Drive(speed, AUTON_TIMED_CURVE_OFFSET); 	// drive forwards 30% speed
		
		while ( driveEncoder->GetDistance() < inches ){
					DEBUG_PRINT("Roto %lf\n", driveEncoder->GetDistance());
				}
								
		leroyDrive->Drive(0.0, 0.0); 	// stop robot

};

	
void RobotDemo::SimpleAutonOne(void){
		
		// This is the simplest autonomous routine without multithreading
		// TODO: Run these concurrently using thread to save time, if needed
		
		ballCollector->On();
		AutonDriveInches( DISTANCE_FROM_KEY_TO_WALL - DISTANCE_BUMPER_WIDTH, 0.30 );
		
};
	
	
void RobotDemo::Autonomous(void){
		
		leroyDrive->SetSafetyEnabled(false);
		
		SimpleAutonOne();
		

};

	
void RobotDemo::OperatorControl(void){
		
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
		
		
};



START_ROBOT_CLASS(RobotDemo);

