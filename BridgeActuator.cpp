#include "BridgeActuator.h"


BridgeActuator::BridgeActuator(void){
	
	relay = new Relay(SPIKE_BRIDGE_ACTUATOR);
	
	relay->Set(Relay::kOff);
	
	inMotion = false;
	deploying = false;
	//wrapped = false;
	isDown = false;
	
	bridgeSensor = new AnalogChannel(ANALOG_INPUT_BRIDGE);
	
	
}

float BridgeActuator::GetRawVoltage(){
	
	float rval = bridgeSensor->GetVoltage();
	
	return rval;
	
}

void BridgeActuator::Deploy(void){
	
	deploying = true;
	inMotion = true;
	isDown= false;
	
	
}

void BridgeActuator::Reset(void){
	
	deploying = false;
	inMotion = false;
}


void BridgeActuator::Undeploy(void){
	
	
	deploying = false;
	inMotion = true;
	
}

void BridgeActuator::Toggle(void){
	
	deploying = !deploying;
	inMotion = true;
	
}


void BridgeActuator::Forward(void){
	
	
	// Forward is UP
	//stop if you passed the top and are not wrapped
	//if ( bridgeSensor->GetVoltage() > BRIDGE_SLAPPER_UPPER_V && !wrapped ) {
		if ( bridgeSensor->GetVoltage() > BRIDGE_SLAPPER_UPPER_V) {
			Off();
			return;
		}
	
	//printf("F Volts: %f\n", bridgeSensor->GetVoltage());
	relay->Set(Relay::kForward);
}


void BridgeActuator::BackwardUnsafe(void){
	
	// Backward is DOWN

	relay->Set(Relay::kReverse);
}

void BridgeActuator::ForwardUnsafe(void){
	
	// Forward is UP
	
	

	relay->Set(Relay::kForward);
}

void BridgeActuator::Backward(void){
	
	// Backward is DOWN
	//stop if you are lower than the top and are wrapped
	//if ( bridgeSensor->GetVoltage() < BRIDGE_SLAPPER_UPPER_V && wrapped ) {//is lower than the upper limit, and has wrapped around 0->5
	if ( bridgeSensor->GetVoltage() < BRIDGE_SLAPPER_LOWER_V  ) {//is lower than the upper limit, and has wrapped around 0->5
		Off();
		isDown = true;
		return;
	}
	
	//printf("B Volts: %f\n", bridgeSensor->GetVoltage());
	isDown = false;
	relay->Set(Relay::kReverse);
}

void BridgeActuator::OffIfNotDeploying(void){
	
	if (!inMotion)
		Off();
}

void BridgeActuator::Off(void){
	
	//DEBUG_PRINT("BRIDGE OFF\n");
	printf("O Volts: %f\n", bridgeSensor->GetVoltage());
	relay->Set(Relay::kOff);
	inMotion = false;
}

void BridgeActuator::Update(){
	
	
	if (!inMotion)
		return;
	
	/*
	if (bridgeSensor->GetVoltage() >= 0 && bridgeSensor->GetVoltage() <= BRIDGE_SLAPPER_LOWER_V && wrapped)
		wrapped = false;
	
	if (bridgeSensor->GetVoltage() <= 5 && bridgeSensor->GetVoltage() >= BRIDGE_SLAPPER_UPPER_V && !wrapped)
		wrapped = true;
	
	if (deploying){
		Backward();
	}
	else // undeploying
	{
		Forward();
	}
	
	*/
	
	
		if (deploying){
			Backward();
		}
		else // undeploying
		{
			Forward();
		}
	
	
	
}