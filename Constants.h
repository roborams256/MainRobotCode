/*
 * Constants.h
 * 
 * Includes #defines for various PWM, Digital I/O Channels, etc.
 * 
 * 
 * Leroy's Variable Speed Motors (on Jaguars):
 * 	
 * 	One PWM on Y: Drive Front Left  /  Drive Back Left
 * 	One PWM on Y: Drive Front Right /  Drive Back Right
 * 	 	
 *  Launcher Right Wheel  /  Launcher Left Wheel
 *  
 *  Launcher Angle Adjust
 *  
 * Leroy's Single Speed Motors (on Spikes):
 * 
 *  Ball Sweep
 *  Ball Trigger Wheel
 * 
 */

// PWM Channels


// The left and right motor banks share a PWM output using
// a Y connector
#define		PWM_LEFT_DRIVE	 	1
#define 	PWM_RIGHT_DRIVE	 	2

#define		PWM_LAUNCHER_LEFT	5
#define		PWM_LAUNCHER_RIGHT	6
#define		PWM_LAUNCH_ANGLE	7


// Spike Shannels

#define		SPIKE_SWEEPER		2
#define		SPIKE_TRIGGER		7



// Digital Outputs

// Laser might be on a spike or a digital IO

#define		SPIKE_LASER			3		

#define		DOUT_LASER			1

/*
 * 
 * Sensors
 * 
 * 	Rotation:
 * 		* Transmission left
 * 		* Transmission right
 * 		* Launch angle
 * 		
 * 	Opto:
 * 		* Ball counter
 * 		
 * 	Microswitch:
 * 		* Launch angle limit
 * 		
 * 	Positional:
 * 		* Gyro
 * 		* Accelerometer
 * 		
 * 
 */

#define 	DIN_LAUNCH_LIMIT	2

/*
 * Gamepad channels
 * 
 * 
Raw Axis 1 = Left X (left is negative)
Raw Axis 2 = Left Y (up is negative)
Raw Axis 3 = Left trigger (0 to 1)
	     Right trigger (0 to -1)
Raw Axis 4 = Right X (left is neg)
Raw Axis 5 = Right Y

Raw 6 = D-Pad L/R

GetThrottle() reads analog triggers (positive right, neg left)
*/

// Scaling factor controls what percent of the joystick motion gets applied
// to the tank drive. 1.0 = 100%, 0.5 = 50% etc.
#define JOYSTICK_DRIVE_SCALE_FACTOR 1.0

#define GPAD_LEFT_Y_RAW_AXIS 	2
#define GPAD_RIGHT_Y_RAW_AXIS 	5

