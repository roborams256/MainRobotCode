#include "Sweeper.h"

Sweeper::Sweeper(int channelA, int channelB)
{
	//Set up the spike relays
	relayA = new Relay(channelA, Relay::kBothDirections);
	relayB = new Relay(channelB, Relay::kBothDirections);
	
	motorADirection = Relay::kOff;
	motorBDirection = Relay::kOff;
	
	relayA->Set(motorADirection);
	relayB->Set(motorBDirection);
	
	isOn = false;
	
	printf("Sweeper initted on channels %d and %d\n", channelA, channelB);

}

void Sweeper::On(void)
{
	motorADirection = Relay::kForward;
	motorBDirection = Relay::kForward;
	
	relayA->Set(motorADirection);
	relayB->Set(motorBDirection);
	isOn = true;
	printf("Sweeper on!\n");
}

void Sweeper::Reverse(void)
{
	motorADirection = Relay::kReverse;
	motorBDirection = Relay::kReverse;
	isOn = true;
	relayA->Set(motorADirection);
	relayB->Set(motorBDirection);
}

void Sweeper::Off(void)
{
	motorADirection = Relay::kOff;
	motorBDirection = Relay::kOff;
		
	relayA->Set(motorADirection);
	relayB->Set(motorBDirection);
	isOn = false;
	printf("Sweeper off!\n");
}

void Sweeper::DirectControl(Relay::Value newADirection, Relay::Value newBDirection)
{
	motorADirection = newADirection;
	motorBDirection = newBDirection;
			
	relayA->Set(motorADirection);
	relayB->Set(motorBDirection);
	
	isOn = true; // TODO should check if motors are not = 0
}

Relay::Value Sweeper::GetMotorA(void)
{
	return motorADirection;
}

Relay::Value Sweeper::GetMotorB(void)
{
	return motorBDirection;
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

