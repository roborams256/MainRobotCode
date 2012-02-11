#include "TriggerWheel.h"


TriggerWheel::TriggerWheel(int spikeChannel, double launchTime){

	triggerRelay = new Relay(SPIKE_TRIGGER);  // default should be both directions
	
	// start in the hold direction
	hold();
	
	launchPeriod = launchTime; 
	semi = false;
	
	timer = new Timer();
	timer->Reset();
	
}

void TriggerWheel::hold(void){
		
	triggerRelay->Set(Relay::kForward);
		
}

void TriggerWheel::fireAuto(void){
		

	triggerRelay->Set(Relay::kReverse);

		
}


void TriggerWheel::setLaunchPeriod(double launchTime)
{
	
	launchPeriod = launchTime;
	
}

void TriggerWheel::fireSemiAuto()
{
	if (semi) return; //don't allow re-firing
	timer->Reset();
	timer->Start();
	
	fireAuto();
	semi = true;
}

void TriggerWheel::Update()
{
	if (semi && timer->HasPeriodPassed(launchPeriod))
	{
		hold();
		timer->Stop();
		timer->Reset();
		semi = false;
	}
}
