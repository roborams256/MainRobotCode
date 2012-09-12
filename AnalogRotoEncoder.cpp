#include "AnalogRotoEncoder.h"




AnalogRotoEncoder::AnalogRotoEncoder(UINT32 analogChannel){
	
	analogInput = new AnalogChannel(analogChannel);
	
	
}
	
float AnalogRotoEncoder::GetAngle(){
	
	return angle;
	
}

	

void AnalogRotoEncoder::Update(){
	
	
	GetRaw();
	
	
}

// TODO this code is crap and has replications from fixing autocal

float AnalogRotoEncoder::GetRaw(){
	
	float rval = analogInput->GetVoltage();
	
	float vdelta = analogInput->GetVoltage() - zeroAngleVoltage;
		
	angle = vdelta*DEGREES_PER_VOLT;
	
	return rval;
	
}

void AnalogRotoEncoder::AutoSetAngleFromZeroVoltage(float zeroVoltage){
	
	zeroAngleVoltage = zeroVoltage;
	float vdelta = analogInput->GetVoltage() - zeroAngleVoltage;
		
	angle = vdelta*DEGREES_PER_VOLT;
	
	
}

void AnalogRotoEncoder::SetStartAngle(void){
	
	// Call this method to set the starting voltage for the encoder at the upper limit
	//angle = UPPER_LIMIT_ANGLE_DEGREES;
	
	//angle = UPPER_LIMIT_ANGLE_DEGREES;
	//upLimitVoltage = analogInput->GetVoltage();
	
	
}