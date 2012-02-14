#pragma once

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
	bool pulse;
	Timer *timer;
	
public:
	
	TriggerWheel(int spikeChannel, double launnchTime);
	void Hold(void);
	void FireAuto(void);
	void FireSemiAuto(void);
	void FirePulse(void);
	void StopPulse(void);
	bool Pulsing(void);
	void SetLaunchPeriod(double launchTime);
	void Update(void);
	
};
