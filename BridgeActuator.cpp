#include "BridgeActuator.h"

BridgeActuator::BridgeActuator(void){
	
	relay = new Relay(SPIKE_BRIDGE_ACTUATOR);
	
	relay->Set(Relay::kOff);
	
	inMotion = false;
	deploying = false;
	
	lowSensor = new DigitalInput(DIG_IN_BOTTOM_SENSOR);
	highSensor = new DigitalInput(DIG_IN_TOP_SENSOR);
	
	
	
	relay->Set(Relay::kOff);
	
	lcd = DriverStationLCD::GetInstance();
	
}

void BridgeActuator::Deploy(void){
	
	//deploying = true;
	//inMotion = true;
	//relay->Set(Relay::kReverse);
	
	// TODO: This may cause the main operator loop to timeout. If so, we'll need an update method to
	// check the digital input.
	
	lcd->Printf(DriverStationLCD:: kUser_Line5, 1, "Bridge Deployed");
	lcd->UpdateLCD();
			
	while (!highSensor->Get())
		relay->Set(Relay::kReverse);
						
	//relay->Set(Relay::kOff);
	
}


void BridgeActuator::Undeploy(void){
	
	lcd->Printf(DriverStationLCD:: kUser_Line5, 1, "Bridge Undeployed");
	lcd->UpdateLCD();
	
	while (!lowSensor->Get())
			relay->Set(Relay::kForward);
							
		relay->Set(Relay::kOff);
	
}

void BridgeActuator::Update(){
	
	/*DEBUG_PRINT("Bridge update===============\n");
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
	*/
	
}