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

// Get the raw axis and button numbers
#import "GPadConstants.h"

// If VERBOSE_DEBUG is defined, then DEBUG_PRINT is replaced with a printf.
// Otherwise it turns into nothing (for normal run).



#define VERBOSE_DEBUG

#ifdef VERBOSE_DEBUG
#define DEBUG_PRINT(args...) printf(args)
#else
#define DEBUG_PRINT
#endif



// PWM Channels


// The left and right motor banks share a PWM output using
// a Y connector
#define		PWM_LEFT_DRIVE	 	1
#define 	PWM_RIGHT_DRIVE	 	2

#define		PWM_LAUNCHER_LEFT	5
#define		PWM_LAUNCHER_RIGHT	6
#define		PWM_LAUNCH_ANGLE	7


// Ball Collection System constants

#define 	SPIKE_BELT				3
#define		SPIKE_SWEEPER			2

#define		SPIKE_TRIGGER			8
#define 	TRIGGER_WHEEL_PERIOD_S	1.0  // 1 second wheel spin


// Bridge Actuator
#define SPIKE_BRIDGE_ACTUATOR 	5
#define DIG_IN_TOP_SENSOR 		3
#define DIG_IN_BOTTOM_SENSOR 	4


// Shooter
#define ANALOG_INPUT_SHOOTER_ANGLE  4
#define DIG_IN_ZERO_SENSOR 	5	
#define ANGLE_JAG_SCALER 0.15



// Digital Outputs



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

// The roto encoder is on pins 1 and 2
#define DIO_ENCODER_A 1
#define DIO_ENCODER_B 2


#define 	DIG_IN_BALL_SENSOR	4

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
#define JOYSTICK_DRIVE_SCALE_FACTOR 0.4


// Roto encoder

#define WHEEL_DIAMETER 4.0
#define PI 3.1415926
#define DRIVE_GEAR_TEETH 15
#define WHEEL_GEAR_TEETH 24
// use this to adjust the math
#define EMPIRICAL_ADJUST 1.0


// Don't mess with the formula!
#define INCHES_PER_TICK  EMPIRICAL_ADJUST * WHEEL_DIAMETER * PI * DRIVE_GEAR_TEETH / WHEEL_GEAR_TEETH / 360



