#import "WPIlib.h"
#import "AnalogRotoEncoder.h"

class BallCannon{
	
	Jaguar *leftJag;
	Jaguar *rightJag;
	Jaguar *angleJag;
	
	DigitalInput *zeroSensor;
	
	AnalogRotoEncoder *angleEncoder;
	
	bool moving;
	bool calibrating;
	
	
public:
	
	BallCannon(void);
	void SetPower(float);
	void SetAngle(float);
	void Calibrate(void);
	void DirectDriveAngle(float jagVal);
	void Update(void);
	
private:
	void CalibrationLoop(void);
	
};

