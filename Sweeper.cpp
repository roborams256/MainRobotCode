#include "Sweeper.h"


Sweeper::Sweeper(int channel)
{
	//Set up the spike relay
	relay = new Relay(channel, Relay::kBothDirections);
	
	motorDirection = Relay::kOff;
	
	relay->Set(motorDirection);
	
	isOn = false;
	
#ifdef VERBOSE_DEBUG
	printf("Sweeper initted on channels %d\n", channel);
#endif

}

void Sweeper::On(void)
{
	motorDirection = Relay::kForward;
	
	relay->Set(motorDirection);
	isOn = true;

	//DEBUG_PRINT("Sweeper on!\n");
	
}

void Sweeper::Reverse(void)
{
	motorDirection = Relay::kReverse;
	isOn = true;
	relay->Set(motorDirection);
}

void Sweeper::Off(void)
{
	motorDirection = Relay::kOff;
		
	relay->Set(motorDirection);
	isOn = false;
	//printf("Sweeper off!\n");
}

void Sweeper::DirectControl(Relay::Value newADirection)
{
	motorDirection = newADirection;
			
	relay->Set(motorDirection);
	
	isOn = true; // TODO should check if motors are not = 0
}

Relay::Value Sweeper::GetMotor(void)
{
	return motorDirection;
}

void Sweeper::InvertDirection(){
	
	printf("Invert called in sweeper.");
	if (motorDirection == Relay::kReverse){
		
		printf("Switching from reverse to forward.\n");
		On();
	
	//if the moter is in reverse it starts to move normally
	}
	else if (motorDirection == Relay::kOn){
		printf("Switching from forward to reverse.\n");
		Reverse();
//if the motor is in on it starts to move in reverse	
	}
}

