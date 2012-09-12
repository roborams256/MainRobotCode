#include "TriggerWheel.h"



TriggerWheel::TriggerWheel(int spikeChannel, double launchTime){

	triggerRelay = new Relay(SPIKE_TRIGGER);  // default should be both directions
	
	// start in the hold direction
	//Hold();
	
	launchPeriod = launchTime; 
	semi = false;
	pulse = false;
	
	timer = new Timer();
	timer->Reset();
	
}

void TriggerWheel::Off(void){
		
	triggerRelay->Set(Relay::kOff);
		
}

void TriggerWheel::Hold(void){
		
	// was kReverse with Ice wheel
	triggerRelay->Set(Relay::kReverse);
		
}

void TriggerWheel::FireAuto(void){
		
	triggerRelay->Set(Relay::kForward);
	DEBUG_PRINT("FIRE auto\n");
		
}


void TriggerWheel::SetLaunchPeriod(double launchTime)
{
	
	launchPeriod = launchTime;
	
}

void TriggerWheel::FireSemiAuto()
{
	
	DEBUG_PRINT("FIRE semiauto\n");

	
	if (semi) return; //don't allow re-firing
	
	timer->Stop();
	timer->Reset();
	timer->Start();
	
	FireAuto();
	semi = true;
	
}



bool TriggerWheel::IsFiring(){
	return semi;
}

void TriggerWheel::Update()
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
