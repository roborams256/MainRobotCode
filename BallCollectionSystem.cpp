
#import "BallCollectionSystem.h"



BallCollectionSystem::BallCollectionSystem(void){
	
	
	ballBelt = new BallBelt(SPIKE_BELT);
	ballBelt->Off();
	
	sweeper = new Sweeper(SPIKE_SWEEPER);
	sweeper->Off();
	
	
	triggerWheel = new TriggerWheel(SPIKE_TRIGGER, TRIGGER_WHEEL_PERIOD_S);
	
	
	//TODO Logic for counter
	
	//ballsInChute = 0;
	
	//ballSensor = new DigitalInput(DIG_IN_BALL_SENSOR);
	
	//ballTimer = new Timer();
	
}

void BallCollectionSystem::On(void){
	
	triggerWheel->Hold();
	sweeper->On();
	ballBelt->On();
}

void BallCollectionSystem::Off(void){
	
	triggerWheel->Hold();
	sweeper->Off();
	ballBelt->Off();
}

void BallCollectionSystem::TotallyOff(void){
	
	triggerWheel->Off();
	sweeper->Off();
	ballBelt->Off();
}


void BallCollectionSystem::Fire(void){
	
	triggerWheel->FireSemiAuto();
	
}

void BallCollectionSystem::FireAuto(void){
	
	triggerWheel->FireAuto();
	
}

void BallCollectionSystem::Hold(void){
	
	triggerWheel->Hold();
	
}

void BallCollectionSystem::Reverse(void){
	
	triggerWheel->Hold();
	sweeper->Reverse();
	ballBelt->Reverse();
}

bool BallCollectionSystem::IsFiring(void){
	
	return triggerWheel->IsFiring();
}

void BallCollectionSystem::Update(void){
	
	
	triggerWheel->Update();
	
}



