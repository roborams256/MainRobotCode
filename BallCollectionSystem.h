#import "WPILib.h"
#import "Sweeper.h"
#import "BallBelt.h"

/*
 * 
 * BallCollectionSystem
 * 
 * The systme includes control of the sweeper and the belt and
 * supports feedback from the 3 ball sensor (if enabled).
 * 
 */


class BallCollectionSystem {
	
public:
	
	int ballsInChute;
	BallBelt *ballBelt;
	Sweeper *sweeper;
	
	BallCollectionSystem(void);
	
	void On(void);
	void Off(void);
	void Reverse(void);
	
	void AutoPause(bool shouldPause);
	void Update();
	
	DigitalInput *ballSensor;
	Timer *ballTimer;
	
	
	
};
