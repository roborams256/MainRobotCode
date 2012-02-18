#include "BallCannon.h"
#include <math.h>
// negative tilts up (increasing angle)
// positive jag goes down
#define OK_ANGLE_ERROR 1.0
#define PROP_DRIVE 0.30

BallCannon::BallCannon(void){
		
	leftJag = new Jaguar( PWM_LAUNCHER_LEFT );
	rightJag = new Jaguar( PWM_LAUNCHER_RIGHT );
	angleJag = new Jaguar( PWM_LAUNCH_ANGLE );
	
	zeroSensor = new DigitalInput(DIG_IN_ZERO_SENSOR);
	
	angleEncoder = new AnalogRotoEncoder( ANALOG_INPUT_SHOOTER_ANGLE );
	
	SetPower(0.25);
	
	calibrating = false;
	moving = false;
	atTargetAngle=false;
	isCalibrated = false;
	
	//temp hack
	//angleEncoder->SetStartAngle();
	
	
}

float BallCannon::GetCurrentAngle(void){
	
	float rval = angleEncoder->GetAngle();
	return rval;
	
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
		isCalibrated = true;
	}	
	else 
	{
		moving = true;
		calibrating = true;
		angleJag->Set(-0.75);
	}
	
}


void BallCannon::Update(void){
	
	angleEncoder->Update();
	
	if ( calibrating ) {
		//DEBUG_PRINT("BC update calibrate\n");
		CalibrationLoop();
		return;
	}
	
	if ( moving ){
		
		float currentAngle = angleEncoder->GetAngle();
		
		float absoluteError = fabs(currentAngle-targetAngle);
		
		if ( absoluteError < OK_ANGLE_ERROR ){
			// we're there
			angleJag->Set(0.0);
			moving = false;
			atTargetAngle=true;
		}
		else if (currentAngle > targetAngle) {
			// We are above. Need to drive forward at a percentage of the error
			DirectDriveAngle(0.50); 
		} 
		else if (currentAngle < targetAngle) {
			// we are below, need to drive backwards
			DirectDriveAngle(-0.50); 
		}
	
	}
	
	
	
}

void BallCannon::AutoUpdateMoving(){
	
	while (moving)
		Update();
}

void BallCannon::SetAngle(float tAngle){
	
	if (!isCalibrated)
		return; // can't work if not calibrated
	
	targetAngle = tAngle;
	moving = true;
	atTargetAngle=false;
	
	
}

void BallCannon::Calibrate(void){
	
	calibrating = true;
	
}

void BallCannon::CancelCal(void){
	
	calibrating = false;
}

void BallCannon::DirectDriveAngle(float jagVal){
	
	//DEBUG_PRINT("Raw V %1,4f Angle reads %1.4f limit is %d\n", angleEncoder->GetRaw(), angleEncoder->GetAngle(), zeroSensor->Get());
	//if ( !zeroSensor->Get() && ( jagVal > 0 ) )
	//	return;
	if (!isCalibrated)
		return; // can't use if not cal'd
	
	if (!calibrating ) { // This is user motion, not auto{
		// Soft limits
		//if ((angleEncoder->GetAngle() > 60.0) && (jagVal < 0))
		//			return;
		
		//if ((angleEncoder->GetAngle() < -20.0) && (jagVal > 0) )
		//			return;
		
		angleJag->Set( ANGLE_JAG_SCALER * jagVal );
			
	}
		
			
			
			
		
	
};

