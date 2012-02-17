#include "WPILib.h"
#include "Constants.h"

/*
 * 
 * AnalogRotoEncoder
 * 
 * Hooks up to the wheel cannon.
 * 
 */

class AnalogRotoEncoder {
	
	AnalogChannel *analogInput;
	float lastReading;
	float upLimitVoltage;
	float angle;
	
	
	
	
public:
	
	AnalogRotoEncoder(UINT32 analogChannel);
	
	float GetAngle();
	void SetStartAngle();
	void Update();
	float GetRaw();
	
	
};