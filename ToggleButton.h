#pragma once

#include "WPILib.h"

/*
 * ToggleButton.hpp
 *
 *  Created on: Feb 9, 2012
 *      Author: Owner
 */

/*
 * Description:
 */
class ToggleButton
{
public:
	/*
	 * Default constructor
	 */
	//ToggleButton(Joystick *gamePad, int button);
	ToggleButton(Joystick *gamePad, int button, int numstates);

	/*
	 * Default destructor
	 */
	~ToggleButton();
	
	void Update(); //handles all the toggling logic
	int State(); //does whatever the button is supposed to do when it is turned on/off
	int Mode();

protected:

private:
	
	int m_state;
	int m_numstates;
	bool m_armed;
	int m_button;
	Joystick *m_gamePad;

};
