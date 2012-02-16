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
	
}

void BallCannon::SetPower(float power){
	
	leftJag->Set(-power);
	rightJag->Set(-power);
	
}

void BallCannon::CalibrationLoop(void){
	
	if ( !zeroSensor->Get() ){		
		angleJag->Set(0.0);
		angleEncoder->SetStartPosition();
		calibrating = false;
		moving = false;		
	}	
}


void BallCannon::Update(void){
	
	angleEncoder->Update();
	
	if ( calibrating ) {
		CalibrationLoop();
		return;
	}
	
	if ( moving ){
		
	}
	
}

void BallCannon::SetAngle(float){
	
}

void BallCannon::Calibrate(void){
	
}

void BallCannon::DirectDriveAngle(float jagVal){
	
	DEBUG_PRINT("Cannon driven with: %1.3f sensor reads %d\n", jagVal, zeroSensor->Get());
	//if ( !zeroSensor->Get() && ( jagVal > 0 ) )
	//	return;
	
	angleJag->Set( ANGLE_JAG_SCALER * jagVal );
	
	
}