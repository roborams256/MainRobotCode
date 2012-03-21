/*
 * LaunchAngle.cpp
 *
 *  Created on: Feb 11, 2012
 *      Author: Owner
 */


#include "WPILib.h"
#include "LaunchAngle.h"
#define MAX_LEFT 0.5
#define MAX_RIGHT 0.95

/*
 * Default constructor
 */
LaunchAngle::LaunchAngle(int channel) {
	encoder = new AnalogChannel(channel);
	lastEncoder = -9999;
	turnNumber = 0;
	thisEncoder = 0;
	finalEncoder = 0;
	changed_turn_number = false;
}

/*
 * Default destructor
 */
LaunchAngle::~LaunchAngle() {

}

void LaunchAngle::read() {
	//thisShoulderEncoder = shoulderEncoder->GetVoltage();
	//thisShoulderEncoder = shoulderEncoder->GetAverageValue(); 
	thisEncoder = encoder->GetValue();

	if (lastEncoder == -9999) { //Starting position.
		turnNumber = 0;
		lastEncoder = thisEncoder;
		printf("initing encoder\n");
	} else {
		// if going forward and encoders change relation
		if (!changed_turn_number) {
			if ((lastEncoder - thisEncoder)>450) {
				turnNumber++;
				changed_turn_number = true;
				/*
				 printf ("shoulderTurnNumber = %d\n", shoulderTurnNumber);
				 */
			};
			// if going backwards and encoder values change direction sense
			if ((thisEncoder - lastEncoder)>450) {
				turnNumber--;
				if (turnNumber < 0) {
					turnNumber = 0;
				}
				changed_turn_number = true;
				/*
				 printf ("shoulderTurnNumber = %d\n", shoulderTurnNumber);
				 printf ("thisShoulderEncoder = %d\n", thisShoulderEncoder);
				 printf ("lastShoulderEncoder = %d\n", lastShoulderEncoder);
				 */
			}
		} else {
			changed_turn_number = false;
		}
	}
	lastEncoder = thisEncoder;
	finalEncoder = thisEncoder + turnNumber * 964;
}

float LaunchAngle::drive(int desired) {
	int diff;
	float new_lefty;

	diff = desired - finalEncoder;
	if (diff < 0) {
		if (diff < -100) {
			new_lefty = MAX_LEFT; // max 
		} else {
			if (diff < -20) {
				new_lefty = 0.1;
			} else {
				new_lefty = 0.0; // stop
			}
		}
	} else {
		if (diff > 100) {
			new_lefty = -MAX_LEFT; // max 
		} else {
			if (diff > 20) {
				new_lefty = -0.1;
			} else {
				new_lefty = 0.0; //stopp
			}
		}
	}
	return new_lefty;
}

void LaunchAngle::Update()
{
	read();
}