#include "WPILib.h"
#include "ToggleButton.hpp"
#include "TriggerWheel.h"
#include "TestHarness.h"
/*
 * TestHarness.cpp
 * 
 * This runs the operator controlled loop in TEST_MODE
 * 
 * 
*/


	TestHarness::TestHarness(Joystick *js){
		
		victorOne = new Victor(10);
		victorOneValue = 0;
		
		joystick = js;
		toggleButton = new ToggleButton(joystick, 1, 4);
		wheel = new TriggerWheel(1, 1);
			
	}
	
	void TestHarness::Update(void){
		
		//victorOne->Set(victorOneValue);
		//printf("Updating Vone with [%f]\n", victorOneValue);
		toggleButton->Update();
		int st = toggleButton->State();
		printf("Button is at state [%d]\n", st);
		victorOne->Set(st == 1 ? -1 : ((st == 2 || st == 4) ? victorOneValue : 1));
		if (joystick->GetRawButton(2))
			wheel->fireSemiAuto();
		
		wheel->Update();
		
	}
	
	
	

