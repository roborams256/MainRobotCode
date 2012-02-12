#include "TriggerWheel.h"


TriggerWheel::TriggerWheel(int spikeChannel, double launchTime){

	triggerRelay = new Relay(SPIKE_TRIGGER);  // default should be both directions
	
	// start in the hold direction
	hold();
	
	launchPeriod = launchTime; 
	semi = false;
	pulse = false;
	
	timer = new Timer();
	timer->Reset();
	
}

void TriggerWheel::hold(void){
		
	triggerRelay->Set(Relay::kReverse);
		
}

void TriggerWheel::fireAuto(void){
		

	triggerRelay->Set(Relay::kForward);

		
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
	printf("BANG\n");
}

void TriggerWheel::firePulse(void)
{
	pulse = true;
	timer->Reset();
	timer->Start();
}

void TriggerWheel::stopPulse(void)
{
	pulse = false;
	timer->Stop();
	timer->Reset();
}

bool TriggerWheel::pulsing(void)
{
	return pulse;
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
	
	if (pulse)
	{
		int result = static_cast<int>(timer->Get() / launchPeriod);
		float mod = timer->Get() - static_cast<float>(result) * launchPeriod;
		triggerRelay->Set((mod > (launchPeriod / 2)) ? (Relay::kOff) : (Relay::kForward));
	}
	
	if (!pulse && !semi)
		stopPulse();
}
