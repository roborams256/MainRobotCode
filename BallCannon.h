#import "WPIlib.h"
#import "AnalogRotoEncoder.h"

class BallCannon{
	
	Jaguar *leftJag;
	Jaguar *rightJag;
	Jaguar *angleJag;
	
	DigitalInput *zeroSensor;
	
	AnalogRotoEncoder *angleEncoder;
	

	
	float targetAngle;
	
	
public:
	
	bool isCalibrated;
	bool atTargetAngle;
	bool moving;
	bool calibrating;
	BallCannon(void);
	void SetPower(float);
	void SetAngle(float);
	void Calibrate(void);
	void CancelCal(void);
	void DirectDriveAngle(float jagVal);
	void Update(void);
	void AutoUpdateMoving(void);
	float GetCurrentAngle(void);
	int GetLimitSensor(void);
	
private:
	void CalibrationLoop(void);
	
};

