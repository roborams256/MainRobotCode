#include "TriggerWheel.h"


TriggerWheel::TriggerWheel(int spikeChannel, double launchTime){

	triggerRelay = new Relay(SPIKE_TRIGGER);  // default should be both directions
	
	// start in the hold direction
	Hold();
	
	launchPeriod = launchTime; 
	semi = false;
	pulse = false;
	
	timer = new Timer();
	timer->Reset();
	
}

void TriggerWheel::Hold(void){
		
	triggerRelay->Set(Relay::kReverse);
		
}

void TriggerWheel::FireAuto(void){
		
	triggerRelay->Set(Relay::kForward);
		
}


void TriggerWheel::SetLaunchPeriod(double launchTime)
{
	
	launchPeriod = launchTime;
	
}

void TriggerWheel::FireSemiAuto()
{
	if (semi) return; //don't allow re-firing
	
	timer->Stop();
	timer->Reset();
	timer->Start();
	
	FireAuto();
	semi = true;
	DEBUG_PRINT("BANG\n");
}

void TriggerWheel::FirePulse(void)
{
	pulse = true;
	timer->Reset();
	timer->Start();
}

void TriggerWheel::StopPulse(void)
{
	pulse = false;
	timer->Stop();
	timer->Reset();
}

bool TriggerWheel::Pulsing(void)
{
	return pulse;
}

void TriggerWheel::Update()
{
	if (semi && timer->HasPeriodPassed(launchPeriod))
	{
		Hold();
		timer->Stop();
		timer->Reset();
		semi = false;
	}
	
	return;
	
	// TODO: Robert, what is pulse?
	
	if (pulse)
	{
		int result = static_cast<int>(timer->Get() / launchPeriod);
		float mod = timer->Get() - static_cast<float>(result) * launchPeriod;
		triggerRelay->Set((mod > (launchPeriod / 2)) ? (Relay::kOff) : (Relay::kForward));
	}
	
	if (!pulse && !semi)
		StopPulse();
}
