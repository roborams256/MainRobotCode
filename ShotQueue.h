#ifndef SHOT_QUEUE
#define SHOT_QUEUE

#include "WPILib.h"
#include "BallCannon.h"
#include "BallCollectionSystem.h"
#include "Shot.h"
#include "Constants.h"
#include <queue>

class ShotQueue {
	
private:
	
	std::queue<Shot *> 		shots;
	BallCollectionSystem 	*ballCollector;
	BallCannon 				*ballCannon;
	
public:
	
	enum States {
		kWaitingForShot,
		kAdjustingSpeed,
		kAdjustingAngle,
		kTakeShot,
		kShotInProgress	,
		kRetiringShot
	};
	
	States currentState;
	Timer *time;
	
	ShotQueue(BallCollectionSystem *bcs, BallCannon *bcan);
	int AddShot(Shot *shot);
	void Update(void);
	
};

#endif