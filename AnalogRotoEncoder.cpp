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



float AnalogRotoEncoder::GetRaw(){
	
	float rval = analogInput->GetVoltage();
	
	angle = UPPER_LIMIT_ANGLE_DEGREES +(rval-upLimitVoltage)*DEGREES_PER_VOLT;
	
	return rval;
	
}

void AnalogRotoEncoder::SetStartAngle(void){
	
	// Call this method to set the starting voltage for the encoder at the upper limit
	//angle = UPPER_LIMIT_ANGLE_DEGREES;
	
	angle = UPPER_LIMIT_ANGLE_DEGREES;
	upLimitVoltage = analogInput->GetVoltage();
	
	
}