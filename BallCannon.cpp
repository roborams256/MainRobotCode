#include "BallCannon.h"
#include <math.h>
// negative tilts up (increasing angle)
// positive jag goes down
#define OK_ANGLE_ERROR 0.5
#define PROP_DRIVE 0.30


BallCannon::BallCannon(void){
		
	leftJag = new Jaguar( PWM_LAUNCHER_LEFT );
	rightJag = new Jaguar( PWM_LAUNCHER_RIGHT );
	angleJag = new Jaguar( PWM_LAUNCH_ANGLE );
	
	zeroSensor = new DigitalInput(DIG_IN_ZERO_SENSOR);
	
	angleEncoder = new AnalogRotoEncoder( ANALOG_INPUT_SHOOTER_ANGLE );
	
	
	angleJag->Set(0.0);
	angleEncoder->AutoSetAngleFromZeroVoltage(ZERO_ANGLE_VOLTAGE);
	calibrating = false;
	moving = false;		
	isCalibrated = true;
		
	spinupTimer = new Timer();
	
	SetSpeed(0.25);
}

float BallCannon::GetSpeed(void){
	
	return ballSpeed;
}

int BallCannon::GetLimitSensor(void){
	
	return zeroSensor->Get();
}

float BallCannon::GetCurrentAngle(void){
	
	float rval = angleEncoder->GetAngle();
	return rval;
	
}

void BallCannon::SetSpeed(float speed){
	
	spinupTimer->Reset();
	spinupTimer->Start();
	
	spinupDelta = fabs(ballSpeed - speed);
	
	ballSpeed = speed;
	leftJag->Set(speed);
	rightJag->Set(speed);
	
}

bool BallCannon::AtSpeed(void){
	
	return spinupTimer->HasPeriodPassed(spinupDelta*2.5);
	
}

void BallCannon::CalibrationLoop(void){
	
	// New code without limit switch cal 3-24-2012
	angleJag->Set(0.0);
	angleEncoder->AutoSetAngleFromZeroVoltage(ZERO_ANGLE_VOLTAGE);
	calibrating = false;
	moving = false;		
	isCalibrated = true;
	return;
	
	// Old code below
	
	if ( !zeroSensor->Get() && calibrating ){		
		angleJag->Set(0.0);
		angleEncoder->SetStartAngle();
		calibrating = false;
		moving = false;		
		isCalibrated = true;
		return;
	}	
	else 
	{
		moving = true;
		calibrating = true;
		angleJag->Set(0.25);
	}
	
	// This is emergency stop
	if (angleEncoder->GetRaw()<1.8){
		angleJag->Set(0.0);
		calibrating = false;
		isCalibrated = false;
		moving = false;
	}
	
}


void BallCannon::Update(void){
	
	angleEncoder->Update();
	
	if ( moving ){
		
		float currentAngle = angleEncoder->GetAngle();
		
		float absoluteError = fabs(currentAngle-targetAngle);
		
		DEBUG_PRINT("Target: %1.2f Angle: %1.2f Error: %1.2f",targetAngle, currentAngle, absoluteError);
		
		if ( absoluteError < OK_ANGLE_ERROR ){
			// we're there
			angleJag->Set(0.0);
			moving = false;
			atTargetAngle=true;
		}
		else if (currentAngle > targetAngle) {
			// We are above. Need to drive forward at a percentage of the error
			DirectDriveAngle(1.0*absoluteError/10.0); 
		} 
		else if (currentAngle < targetAngle) {
			// we are below, need to drive backwards
			DirectDriveAngle(-1.0*absoluteError/10.0); 
		}
	
	}
	
	
	
}

void BallCannon::AutoUpdateMoving(){
	
	while (moving)
		Update();
}

void BallCannon::SetAngle(float tAngle){
	
	
	targetAngle = tAngle;
	moving = true;
	atTargetAngle=false;
	
	
}

void BallCannon::PIDSetAngle(float angle){
	
	// not implemented, not needed right now
}

void BallCannon::Calibrate(void){
	
	// 2.3V is 0 degrees
	isCalibrated = true;
	return;
	
}

void BallCannon::CancelCal(void){
	
	calibrating = false;
}

void BallCannon::DirectDriveAngle(float jagVal){

	DEBUG_PRINT("Shooter angle %1.2f and voltage %1.2f and jagVal %1.2f\n", angleEncoder->GetAngle(),
			angleEncoder->GetRaw(), jagVal);
	
	
	// Positive jagVal is tilting head forward to a neg angle and smaller, nonzero voltage

	
	if ( jagVal>0 && angleEncoder->GetRaw()< 1.0 ){
		printf("Past limit with voltage %1.2f and jagVal %1.2f\n", angleEncoder->GetRaw(),jagVal);
		jagVal = 0;
	}
	// neg jagVal is tilting back to positive angle and larger voltage
	else if (jagVal<0 && angleEncoder->GetRaw() > 4.8 ){
		printf("Past limit with voltage %1.2f and jagVal %1.2f\n", angleEncoder->GetRaw(),jagVal);
		jagVal = 0;
	}
	

	angleJag->Set( jagVal );
			
		
}
			
float BallCannon::GetVoltage(void){
	return angleEncoder->GetRaw();
}
			
		
	

