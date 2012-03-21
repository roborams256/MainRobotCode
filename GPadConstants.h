/*
 * 
 * Constants for the Logitech Gamepad
 * 
 * Don't edit this unless you really understand what you are DOING!!!
 * Edits: rg 2/15/12 - I think the RIGHT_Y_AXIS and RIGHT_X_AXIS were not 3 and 4 -> 4 and 5
 * 
 * 
 * 
 */

#define F310_X_MODE

#ifdef F310_X_MODE

#define X_BUTTON 3
#define A_BUTTON 1
#define B_BUTTON 2
#define Y_BUTTON 4
#define LB_BUTTON 5
#define RB_BUTTON 6

// Left and Right triggers are analog in X mode
//#define RTRIG_BUTTON 7
//#define LTRIG_BUTTON 8

#define BACK_BUTTON 7
#define START_BUTTON 8

#define LEFTZ_BUTTON 9
#define RIGHTZ_BUTTON 10

#define TRIGGER_AXIS 3

#define LEFT_X_AXIS 1
#define LEFT_Y_AXIS 2

#define RIGHT_X_AXIS 4
#define RIGHT_Y_AXIS 5

#define DPAD_X_AXIS 6
//#define DPAD_Y_AXIS 2

#endif


#ifdef F310_D_MODE

#define X_BUTTON 1
#define A_BUTTON 2
#define B_BUTTON 3
#define Y_BUTTON 4
#define LB_BUTTON 5
#define RB_BUTTON 6
#define RTRIG_BUTTON 7
#define LTRIG_BUTTON 8
#define BACK_BUTTON 9
#define START_BUTTON 10
#define LEFTZ_BUTTON 11
#define RIGHTZ_BUTTON 12

#define LEFT_X_AXIS 1
#define LEFT_Y_AXIS 2

#define RIGHT_X_AXIS 4
#define RIGHT_Y_AXIS 5

#define DPAD_X_AXIS 5
#define DPAD_Y_AXIS 6

#endif



