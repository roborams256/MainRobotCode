#import "BallWheels.h"

/*
 * BallWheels.cpp
 *
 *  Created on: Feb 13, 2012
 *      Author: home
 */

/*
 * Description:
 */

BallWheels::BallWheels(int ljag, int rjag){
	
	leftWheelJag = new Jaguar(ljag);
	rightWheelJag = new Jaguar(rjag);
	
	Set(0.0);
	
}

void BallWheels::Set(float speed){
	
	// Negative speeds are used to shoot the ball, this prevents positive speeds
	if (speed > 0)
		speed = -1.0*speed;
	
	leftWheelJag->Set(speed);
	rightWheelJag->Set(speed);
		
}

