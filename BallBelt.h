#include "WPILib.h"
#include "Constants.h"

//

class BallBelt
{
public:
	BallBelt(int channel);
	void On(void);
	void Off(void);
	void Reverse(void);
	void InvertDirection();
	
	bool isOn;
	
	
private:
	
	Relay::Value motorDirection;
	Relay *relay;
	
	
};
