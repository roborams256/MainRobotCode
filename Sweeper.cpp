#include "Sweeper.h"

Sweeper::Sweeper(int channelA)
{
	//Set up the spike relays
	relayA = new Relay(channelA, Relay::kBothDirections);
	
	motorADirection = Relay::kOff;
	
	relayA->Set(motorADirection);
	
	isOn = false;
	
	printf("Sweeper initted on channels %d and %d\n", channelA);

}

void Sweeper::On(void)
{
	motorADirection = Relay::kForward;
	
	relayA->Set(motorADirection);
	isOn = true;
	printf("Sweeper on!\n");
}

void Sweeper::Reverse(void)
{
	motorADirection = Relay::kReverse;
	isOn = true;
	relayA->Set(motorADirection);
}

void Sweeper::Off(void)
{
	motorADirection = Relay::kOff;
		
	relayA->Set(motorADirection);
	isOn = false;
	printf("Sweeper off!\n");
}

void Sweeper::DirectControl(Relay::Value newADirection)
{
	motorADirection = newADirection;
			
	relayA->Set(motorADirection);
	
	isOn = true; // TODO should check if motors are not = 0
}

Relay::Value Sweeper::GetMotor(void)
{
	return motorADirection;
}

void Sweeper::InvertDirection(){
	
	printf("Invert called in sweeper.");
	if (motorADirection == Relay::kReverse){
		
		printf("Switching from reverse to forward.\n");
		On();
	
	//if the moter is in reverse it starts to move normally
	}
	else if (motorADirection == Relay::kOn){
		printf("Switching from forward to reverse.\n");
		Reverse();
//if the motor is in on it starts to move in reverse	
	}
}

