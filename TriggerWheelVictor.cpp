#include "TriggerWheelVictor.h"



TriggerWheelVictor::TriggerWheelVictor(int vicChannel, double launchTime){

	triggerVic = new Victor(VICTOR_TRIGGER);  // default should be both directions
	
	// start in the hold direction
	//Hold();
	
	launchPeriod = launchTime; 
	semi = false;
	
	timer = new Timer();
	timer->Reset();
	
}

void TriggerWheelVictor::Off(void){
		
	triggerVic->Set(0.0);
		
}

void TriggerWheelVictor::Hold(void){
		
	// was kReverse with Ice wheel
	triggerVic->Set(HOLD_TRIGGER_SPEED);
		
}

void TriggerWheelVictor::FireAuto(void){
		
	triggerVic->Set(LAUNCH_TRIGGER_SPEED);
	DEBUG_PRINT("FIRE auto\n");
		
}


void TriggerWheelVictor::SetLaunchPeriod(double launchTime)
{
	
	launchPeriod = launchTime;
	
}

void TriggerWheelVictor::FireSemiAuto()
{
	
	DEBUG_PRINT("FIRE semiauto\n");

	
	if (semi) return; //don't allow re-firing
	
	timer->Stop();
	timer->Reset();
	timer->Start();
	
	FireAuto();
	semi = true;
	
}



bool TriggerWheelVictor::IsFiring(){
	return semi;
}

void TriggerWheelVictor::Update()
{

	//DEBUG_PRINT("Timer %lf\n Semi %d\n",timer->Get(), semi);
	if (semi && timer->HasPeriodPassed(launchPeriod))
	{
		Hold();
		timer->Stop();
		timer->Reset();
		semi = false;
	}
	else{
		//DEBUG_PRINT("Timer: %f\n", timer->Get());
	}
	
	return;
	
	
}
