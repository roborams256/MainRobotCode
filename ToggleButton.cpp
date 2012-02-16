/*
 * ToggleButton.cpp
 *
 *  Created on: Feb 9, 2012
 *      Author: Owner
 */

#include "ToggleButton.h"

/*
 * Default constructor
 */
/*ToggleButton::ToggleButton(Joystick *gamePad, int button)
{
	m_button = button;
	m_gamePad = gamePad;
	m_numstates = 2;
}*/

//Another Constructor
ToggleButton::ToggleButton(Joystick *gamePad, int button, int numstates)
{
	m_button = button;
	m_gamePad = gamePad;
	m_numstates = numstates;
	m_state = 0;  // changed by Mitch to reflect standard of counting from 0
}

/*
 * Default destructor
 */
ToggleButton::~ToggleButton()
{
	
}

//true == true
//!true == false

void ToggleButton::Update()
{
	
	if (m_armed && m_gamePad->GetRawButton(m_button)) // || means "or"
	{
		// Lock it
		m_armed = false;
		m_state++;
		//printf("Button state changed to: [%d]\n", m_state);
		if (m_state > m_numstates)
		{
			m_state = 0;
		}
	}
	
	if(!m_gamePad->GetRawButton(m_button))
		m_armed = true;
}

int ToggleButton::State()
{
	Update();  //added by mitch so you don't have to update every cycle
	return m_state;
}

int ToggleButton::Mode(){
	return State();
}
