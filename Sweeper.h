#include "WPILib.h"

class Sweeper
{
public:
	Sweeper(int channelA);//constructor so it doesn't return anything
	void On(void);
	void Off(void);//returns nothing
	void Reverse(void);//returns nothing
	void DirectControl(Relay::Value newADirection);//directly sets the direction of the motor
	void InvertDirection();
	
	Relay::Value GetMotor(void);//returns an int
	
	bool isOn;
	
	
private:
	
	Relay::Value motorADirection;
	Relay *relayA;
	//typedef enum {Cheese, Bacon, Kangaroo, Barbaque} Burgers;
	
};
