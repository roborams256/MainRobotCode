/*
 * Constants.h
 * 
 * Includes #defines for various PWM, Digital I/O Channels, etc.
 * 
 * 
 */

// Get the raw axis and button numbers
#import "GPadConstants.h"

// If VERBOSE_DEBUG is defined, then DEBUG_PRINT is replaced with a printf.
// Otherwise it turns into nothing (for normal run).



//#define VERBOSE_DEBUG

#ifdef VERBOSE_DEBUG
#define DEBUG_PRINT(args...) printf(args)
#else
#define DEBUG_PRINT
#endif

// dI
#define		SLOW_DRIVE_SCALER	0.8


#define     BRIDGE_SLAPPER_LOWER_V 0.23
#define		BRIDGE_SLAPPER_UPPER_V 4.75

#define		HOLD_TRIGGER_SPEED  -0.4
#define     LAUNCH_TRIGGER_SPEED 1.0
#define 	TRIGGER_WHEEL_PERIOD_S	0.5  // 0.5 second wheel spin


// Shooter


#define ANGLE_JAG_SCALER 0.35
#define ZERO_ANGLE_VOLTAGE 1.39 // Measureds
#define UPPER_LIMIT_ANGLE_DEGREES  -15.0 	// Measured 
#define LOWER_LIMIT_ANGLE_DEGREES   68.0 	// Measured
#define PARALLEL_VOLTAGE  2.15			// MEasured
#define UP_45_VOLTAGE 3.57
#define DEGREES_PER_VOLT	31.69 //(45/(UP_45_VOLTAGE-PARALLEL_VOLTAGE)) // At about parallel we are at about 2V less 




// Roto encoder

#define WHEEL_DIAMETER 4.0
#define PI 3.1415926
#define DRIVE_GEAR_TEETH 15
#define WHEEL_GEAR_TEETH 24
// use this to adjust the math
#define EMPIRICAL_ADJUST (46.0/50.0)  // measured in lab


// Don't mess with the formula!
#define INCHES_PER_TICK  EMPIRICAL_ADJUST * WHEEL_DIAMETER * PI * DRIVE_GEAR_TEETH / WHEEL_GEAR_TEETH / 360


/*
 * 
 * Smooth Drive
 * 
 * */
#define LP_FILTER_TERMS 50


// Shooted preset baselines

// Middle basket from bumper
#define BASELINE_OFFENSE_A_BUTTON_SPEED 0.35
#define BASELINE_OFFENSE_A_BUTTON_ANGLE 58.0

// High Basket from bumper
#define BASELINE_DEFENSE_A_BUTTON_SPEED 0.85
#define BASELINE_DEFENSE_A_BUTTON_ANGLE 45.0



/*******************************
 * 
 * Signal assignments
 * 
 * *****************************/

// The left and right motor banks share a PWM output using
// a Y connector
#define		PWM_LEFT_DRIVE	 	2
#define 	PWM_RIGHT_DRIVE	 	1
#define		PWM_LAUNCHER_LEFT	5
#define		PWM_LAUNCHER_RIGHT	6
#define		PWM_LAUNCH_ANGLE	7
#define 	VICTOR_TRIGGER		4

#define 	SPIKE_BELT				3
#define		SPIKE_SWEEPER			2
#define		SPIKE_TRIGGER			8 // not used anymo
#define 	SPIKE_BRIDGE_ACTUATOR 	1

#define 	ANALOG_INPUT_BRIDGE 	7

#define 	ANALOG_INPUT_SHOOTER_ANGLE  4

#define 	DIG_IN_ZERO_SENSOR 	14	

// The roto encoder is on...
#define		DIO_ENCODER_A 1
#define 	DIO_ENCODER_B 2


#define 	DIG_IN_BALL_SENSOR	14

#define		AUTON_SWITCH_INPUT 	4
#define		AUTON_SWITCH_2 10

#define MULTI_AUTON

//Robert, at Madera 2012
#define		ABM_DISTANCE	40  //91" from the end of the key to the bridge. make sure to compensate for error, the length of the tipper, and other space requirements
//85 is a number I'm just guessing at. We'll probably need to actually do testing to get it right.

#define 	TEAM_BRIDGE_DISTANCE 96 // bit of a guess based on geometry. 108" is the 16' minus key width (48") minus frame width of 36" with some deployer extended

 