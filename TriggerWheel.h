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
	bool pulse;
	Timer *timer;
	
public:
	bool semi;

	
	TriggerWheel(int spikeChannel, double launnchTime);
	void Off(void);
	void Hold(void);
	void FireAuto(void);
	void FireSemiAuto(void);
	bool IsFiring();
	void FirePulse(void);
	void StopPulse(void);
	bool Pulsing(void);
	void SetLaunchPeriod(double launchTime);
	void Update(void);
	
};
