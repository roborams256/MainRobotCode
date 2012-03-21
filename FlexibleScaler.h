#include <deque>
#include "Constants.h"

//

class FlexibleScaler {
	

public:
	
	enum ScaleModes {
		kScaleLinear,
		kScalePower2,
		kLPFilter
	}; 
	

	ScaleModes scaleMode;
	
	float linearScaleValue;
	
	std::deque<float> filterQueue;
	int filterLength;
	
	FlexibleScaler();
	FlexibleScaler(ScaleModes mode, float scale);
	
	float Scale(float input);
	void SetModeAndScaler(ScaleModes mode, float scale);
	void FlexibleScaler::InitFilterQueue();
	
	
};

