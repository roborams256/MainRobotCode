#include "WPILib.h"

class Sweeper
{
public:
	Sweeper(int channelA, int channelB);//constructor so it doesn't return anything
	void On(void);
	void Off(void);//returns nothing
	void Reverse(void);//returns nothing
	void DirectControl(Relay::Value newADirection, Relay::Value newBDirection);//directly sets the direction of the motor
	void InvertDirection();
	
	Relay::Value GetMotorA(void);//returns an int
	Relay::Value GetMotorB(void);
	
	bool isOn;
	
	
private:
	
	Relay::Value motorADirection;
	Relay::Value motorBDirection;
	Relay *relayA;
	Relay *relayB;
	//typedef enum {Cheese, Bacon, Kangaroo, Barbaque} Burgers;
	
};
