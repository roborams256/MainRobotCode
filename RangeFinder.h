#include "WPILIb.h"
#include "Constants.h"

/*
 * 
 */

class RangeFinder{
	
public:
	
	RangeFinder(void);
	AnalogChannel *analogInput;
	float getRangeInInches();
	float getRawVoltage();
	
};
