#include "BridgeActuator.h"

BridgeActuator::BridgeActuator(void){
	
	relay = new Relay(SPIKE_BRIDGE_ACTUATOR);
	
	relay->Set(Relay::kOff);
	
	inMotion = false;
	deploying = false;
	
	lowSensor = new DigitalInput(DIG_IN_BOTTOM_SENSOR);
	highSensor = new DigitalInput(DIG_IN_TOP_SENSOR);
	
	// Park that beeeatch
	while(highSensor->Get())
		relay->Set(Relay::kForward);
	
	relay->Set(Relay::kOff);
	
}

void BridgeActuator::Deploy(void){
	
	deploying = true;
	inMotion = true;
	relay->Set(Relay::kReverse);
	
}


void BridgeActuator::Undeploy(void){
	
	deploying = false;
	inMotion = true;
	relay->Set(Relay::kForward);
	
}

void BridgeActuator::Update(){
	
	if (inMotion){
		if (deploying && !lowSensor->Get()){
			relay->Set(Relay::kOff);
			inMotion = false;
		} 
		else if (!deploying && !highSensor->Get()){
			relay->Set(Relay::kOff);
			inMotion = false;
		}
		                                
			
	}
	
}