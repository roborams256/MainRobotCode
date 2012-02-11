#include "WPILib.h"
#include "Constants.h"

/*
 * 
 * TriggerWheel.h
 * 
 * Class for controlling the trigger wheel.
 * 
 */


class TriggerWheel{
	
private:
	
	//Relay::Value direction;
	
	Relay *triggerRelay;
	double launchPeriod;
	bool semi;
	Timer *timer;
	
public:
	
	TriggerWheel(int spikeChannel, double launnchTime);
	void hold(void);
	void fireAuto(void);
	void fireSemiAuto(void);
	void setLaunchPeriod(double launchTime);
	void Update(void);
	
};
