/*
 * LaunchAngle.hpp
 *
 *  Created on: Feb 11, 2012
 *      Author: Owner
 * 
 */

/*
 * Description:
 */
class LaunchAngle
{
public:
	/*
	 * Default constructor
	 */
	LaunchAngle(int channel);

	/*
	 * Default destructor
	 */
	~LaunchAngle();
	
	void read(void);
	float drive(int desired);
	void Update(void);

protected:

private:
	
	AnalogChannel *encoder;
	int lastEncoder;
	int turnNumber;
	int thisEncoder;
	int finalEncoder;
	bool changed_turn_number;

};
