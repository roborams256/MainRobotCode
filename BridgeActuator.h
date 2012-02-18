#include "WPILib.h"
#include "Constants.h"

class BridgeActuator{
	
public:
	
	DriverStationLCD *lcd;
	Relay *relay;
	bool deploying;
	bool inMotion;
	DigitalInput *highSensor;
	DigitalInput *lowSensor;
	
	BridgeActuator(void);
	void Deploy(void);
	void Undeploy();
	void Update();
	
};
