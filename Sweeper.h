#include "WPILib.h"
#include "Constants.h"



class Sweeper
{
public:
	Sweeper(int channel);
	void On(void);
	void Off(void);
	void Reverse(void);
	void DirectControl(Relay::Value newADirection);//directly sets the direction of the motor
	void InvertDirection();
	
	Relay::Value GetMotor(void);//returns an int
	
	bool isOn;
	
	
private:
	
	Relay::Value motorDirection;
	Relay *relay;
	
	
};
