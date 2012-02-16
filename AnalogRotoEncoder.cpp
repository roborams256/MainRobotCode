#include "AnalogRotoEncoder.h"



AnalogRotoEncoder::AnalogRotoEncoder(UINT32 analogChannel){
	
	analogInput = new AnalogChannel(analogChannel);
	
	
}
	
float AnalogRotoEncoder::GetAngle(){
	
	return 29.0;
	
}

void AnalogRotoEncoder::SetStartPosition(){
	
	turns = 0;
	lastReading = analogInput->GetValue();
	
}
	

void AnalogRotoEncoder::Update(){
	
	
	int thisReading = analogInput->GetValue();
	
	int delta = thisReading - lastReading;
	
	if ( delta > 450 )
		turns++;
	else if ( delta < 450 )
		turns--;
	
	if (turns<0)
		turns = 0; // This should never happen if correctly calibrated
	
	lastReading = thisReading;

	DEBUG_PRINT("ROTO < Last: %d | This: %d | Delta: %d | Turns %d  > \n", 
			lastReading, thisReading, delta, turns);
	
}


float AnalogRotoEncoder::GetRaw(){
	
	return analogInput->GetValue();
	
}