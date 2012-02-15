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
	int lastReading;
	int turns;
	
public:
	
	AnalogRotoEncoder(UINT32 analogChannel);
	
	float GetAngle();
	void SetStartPosition();
	void Update();
	float GetRaw();
	
	
};