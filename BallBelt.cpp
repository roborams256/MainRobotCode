#include "BallBelt.h"

BallBelt::BallBelt(int channel)
{
	//Set up the spike relay
	relay = new Relay(channel, Relay::kBothDirections);
	
	motorDirection = Relay::kOff;
	
	relay->Set(motorDirection);
	
	isOn = false;
	
	DEBUG_PRINT("Belt initted on Spike channel %d\n", channel);

}

void BallBelt::On(void)
{
	motorDirection = Relay::kForward;
	
	relay->Set(motorDirection);
	isOn = true;

	DEBUG_PRINT("Belt on!\n");
	
}

void BallBelt::Reverse(void)
{
	motorDirection = Relay::kReverse;
	isOn = true;
	relay->Set(motorDirection);
	
	DEBUG_PRINT("Belt reversed!\n");
}

void BallBelt::Off(void)
{
	motorDirection = Relay::kOff;
		
	relay->Set(motorDirection);
	isOn = false;
	
	DEBUG_PRINT("Belt off!\n");
}

void BallBelt::InvertDirection(){
	
	DEBUG_PRINT("Invert called on ball belt.");
	if (motorDirection == Relay::kReverse){
		
		DEBUG_PRINT("Switching from reverse to forward.\n");
		On();
	
	//if the moter is in reverse it starts to move normally
	}
	else if (motorDirection == Relay::kOn){
		DEBUG_PRINT("Switching from forward to reverse.\n");
		Reverse();
//if the motor is in on it starts to move in reverse	
	}
}

