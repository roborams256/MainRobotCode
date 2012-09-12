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



class TriggerWheelVictor{
	
private:
	
	//Relay::Value direction;
	
	Victor *triggerVic;
	double launchPeriod;
	Timer *timer;
	
public:
	bool semi;

	
	TriggerWheelVictor(int vicChannel, double launnchTime);
	void Off(void);
	void Hold(void);
	void FireAuto(void);
	void FireSemiAuto(void);
	bool IsFiring();
	void SetLaunchPeriod(double launchTime);
	void Update(void);
	
};
