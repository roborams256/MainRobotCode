#include "TriggerWheel.h"


TriggerWheel::TriggerWheel(int spikeChannel, double launchTime){

	triggerRelay = new Relay(SPIKE_TRIGGER);  // default should be both directions
	
	// start in the hold direction
	hold();
	
	launchPeriod = launchTime; 
	
	timer = new Timer();
	
	
}

void TriggerWheel::hold(void){
		
	triggerRelay->Set(Relay::kForward);
		
}

void TriggerWheel::fireAuto(void){
		

	triggerRelay->Set(Relay::kReverse);

		
}


void TriggerWheel::setLaunchPeriod(double launchTime){
	
	
	
	
}
