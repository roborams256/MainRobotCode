#include "WPILib.h"
#include "Constants.h"

// 

class BridgeActuator{
	
public:
	
	Relay *relay;
	bool deploying;
	bool inMotion;
	bool isDown;
	AnalogChannel *bridgeSensor;
	
	BridgeActuator(void);
	void Deploy(void);
	void Undeploy();
	void Update();
	float GetRawVoltage();
	
	void Backward();
	void BackwardUnsafe(void);
	void Forward();
	void ForwardUnsafe(void);
	void Reset(void);
	void Off();
	void Toggle(void);
	void OffIfNotDeploying(void);
	
};
