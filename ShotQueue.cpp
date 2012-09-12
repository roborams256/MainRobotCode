#include "ShotQueue.h"


ShotQueue::ShotQueue(BallCollectionSystem *bcs, BallCannon *bcan){
	
	ballCannon = bcan;
	ballCollector = bcs;
	currentState = kStartingShot;
	
}

int ShotQueue::AddShot(Shot *shot){
	
	shots.push(shot);
	return shots.size();
	
}

void ShotQueue::PurgeQueue(void){
	
	while (!shots.empty())
		shots.pop();
}

void ShotQueue::Update(void){
	
	// We always process the top of the stack
	
	if (shots.size()==0)
		return;
	
	// There's at least on shot in the queue
	
	switch (currentState){
	
	case kStartingShot:
		// first adjust speed
		float speed = shots.front()->speed;
		float angle = shots.front()->angle;
		DEBUG_PRINT("\nProcessing Shot %1.2f, %1.2f\n", speed, angle);
		ballCannon->SetSpeed(speed);
		ballCannon->SetAngle(angle);
		currentState = kAdjustingSpeed;
		break;
		
	case kAdjustingSpeed:
		
		DEBUG_PRINT(".");
		if (ballCannon->AtSpeed()){
			DEBUG_PRINT("\nAdjusted Speed\n");
			currentState = kAdjustingAngle;
		}
			
		break;
		
	case kAdjustingAngle:
		
		DEBUG_PRINT(".");
		
		if (ballCannon->atTargetAngle){
			DEBUG_PRINT("\nAngle Adjusted\n");
			currentState = kTakeShot;
		}
			
		break;
	
	case kTakeShot:
		DEBUG_PRINT("\nFiring\n");
		ballCollector->Fire();
		currentState = kShotInProgress;
		
	case kShotInProgress:
		
		DEBUG_PRINT("%d", ballCollector->IsFiring());
		if (!ballCollector->IsFiring())
		{
			DEBUG_PRINT("\nRetiring shot\n");
			currentState = kRetiringShot;
		}
			
		break;
		
	case kRetiringShot:
		DEBUG_PRINT("\nPopping shot off stack.\n");
		shots.pop();
		currentState = kStartingShot;
		break;
		
	default:
		break; // should never be here
	}
	
};

//Added at Madera 2012 by Robert
//It's necissary for AutonBridgeMode, and is useful for other things

int ShotQueue::NumShots(void){
	
	return shots.size();
	
}

bool ShotQueue::IsEmpty(void){
	
	return shots.empty();

}