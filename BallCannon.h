#ifndef BALL_CANNON
#define BALL_CANNON

#import "WPIlib.h"
#import "AnalogRotoEncoder.h"

class BallCannon {
	
	Jaguar *leftJag;
	Jaguar *rightJag;
	Jaguar *angleJag;
	
	DigitalInput *zeroSensor;
	
	AnalogRotoEncoder *angleEncoder;
	
	float targetAngle;
	float ballSpeed;
	float spinupDelta;
	
	Timer *spinupTimer;
	
	
public:
	
	bool isCalibrated;
	bool atTargetAngle;
	bool moving;
	bool calibrating;
	

	
	BallCannon(void);
	void SetSpeed(float);
	float GetSpeed(void);
	void SetAngle(float);
	void PIDSetAngle(float);
	void Calibrate(void);
	void CancelCal(void);
	void DirectDriveAngle(float jagVal);
	void Update(void);
	void AutoUpdateMoving(void);
	float GetCurrentAngle(void);
	int GetLimitSensor(void);
	float GetVoltage();
	bool AtSpeed();

	
private:
	void CalibrationLoop(void);
	
};

#endif
