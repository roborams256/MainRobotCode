#include "BallCannon.h"


BallCannon::BallCannon(void){
		
	leftJag = new Jaguar( PWM_LAUNCHER_LEFT );
	rightJag = new Jaguar( PWM_LAUNCHER_RIGHT );
	angleJag = new Jaguar( PWM_LAUNCH_ANGLE );
	
	zeroSensor = new DigitalInput(DIG_IN_ZERO_SENSOR);
	
	angleEncoder = new AnalogRotoEncoder( ANALOG_INPUT_SHOOTER_ANGLE );
	
	SetPower(0.25);
	
	calibrating = false;
	moving = false;
	
	//temp hack
	angleEncoder->SetStartAngle();
	
}

void BallCannon::SetPower(float power){
	
	leftJag->Set(-power);
	rightJag->Set(-power);
	
}

void BallCannon::CalibrationLoop(void){
	
	if ( !zeroSensor->Get() && calibrating ){		
		angleJag->Set(0.0);
		angleEncoder->SetStartAngle();
		calibrating = false;
		moving = false;		
	}	
	else 
	{
		moving = true;
		calibrating = true;
		angleJag->Set(-0.25);
	}
	
}


void BallCannon::Update(void){
	
	angleEncoder->Update();
	
	if ( calibrating ) {
		DEBUG_PRINT("BC update calibrate\n");
		CalibrationLoop();
		return;
	}
	
	if ( moving ){
		
	
	}
	
	DEBUG_PRINT("BC update normal");
	
}

void BallCannon::SetAngle(float tAngle){
	
	targetAngle = tAngle;
	
	
}

void BallCannon::Calibrate(void){
	
	calibrating = true;
	
}

void BallCannon::CancelCal(void){
	
	calibrating = false;
}

void BallCannon::DirectDriveAngle(float jagVal){
	
	DEBUG_PRINT("Raw V %1,4f Angle reads %1.4f limit is %d\n", angleEncoder->GetRaw(), angleEncoder->GetAngle(), zeroSensor->Get());
	//if ( !zeroSensor->Get() && ( jagVal > 0 ) )
	//	return;
	
	if (!calibrating)
		angleJag->Set( ANGLE_JAG_SCALER * jagVal );
	
	
}