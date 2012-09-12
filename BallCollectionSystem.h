#ifndef BCS
#define BCS

#import "WPILib.h"
#import "Sweeper.h"
#import "BallBelt.h"
#import "TriggerWheelVictor.h"

/*
 * 
 * BallCollectionSystem
 * 
 * The systme includes control of the sweeper and the belt and
 * supports feedback from the 3 ball sensor (if enabled).
 * 
 * 
 */



class BallCollectionSystem {
	
public:
	
	int ballsInChute;
	BallBelt *ballBelt;
	Sweeper *sweeper;
	TriggerWheelVictor *triggerWheel;
	
	BallCollectionSystem(void);
	
	void On(void);
	void Off(void);
	void TotallyOff(void);
	void Reverse(void);
	
	void AutoPause(bool shouldPause);
	void Update(void);
	
	void Fire(void);
	void FireAuto(void);
	void Hold(void);
	
	bool IsFiring(void);
	
	
	// TODO implement ball counter
	DigitalInput *ballSensor;
	Timer *ballTimer;
	
	
	
};

#endif
