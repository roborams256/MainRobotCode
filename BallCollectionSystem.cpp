
#import "BallCollectionSystem.h"


BallCollectionSystem::BallCollectionSystem(void){
	
	
	ballBelt = new BallBelt(SPIKE_BELT);
	ballBelt->Off();
	
	sweeper = new Sweeper(SPIKE_SWEEPER);
	sweeper->Off();
	
	ballsInChute = 0;
	
	ballSensor = new DigitalInput(DIG_IN_BALL_SENSOR);
	
	ballTimer = new Timer();
	
}
