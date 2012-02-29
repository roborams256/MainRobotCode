#include "ShotQueue.h"


ShotQueue::ShotQueue(BallCollectionSystem *bcs, BallCannon *bcan){
	
	ballCannon = bcan;
	ballCollector = bcs;
	currentState = kWaitingForShot;
	
}

int ShotQueue::AddShot(Shot *shot){
	
	shots.push(shot);
	return shots.size();
	
}

void ShotQueue::Update(void){
	
	// We always process the top of the stack
	
	if (shots.size()==0)
		return;
	
	// There's at least on shot in the queue
	
	switch (currentState){
	
	case kWaitingForShot:
		// first adjust speed
		DEBUG_PRINT("\nProcessing Shot\n");
		ballCannon->SetSpeed(shots.front()->speed);
		ballCannon->SetAngle(shots.front()->angle);
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
		currentState = kWaitingForShot;
		break;
		
	default:
		break; // should never be here
	}
	
}