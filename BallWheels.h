#import "WPILib.h"

/*
 * BallWheels.h
 *
 *  Created on: Feb 13, 2012
 *      Author: RoboRams
 */

/*
 * Description:  Controls just the wheels.
 * 
 * 
 */

class BallWheels{
	
private:
	
	Jaguar *leftWheelJag;
	Jaguar *rightWheelJag;
	
public:
	
	BallWheels(int leftPWMChannel, int rightPWMChannel);
	void Set(float speed);
	
};
