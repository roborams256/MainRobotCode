
class FlexibleScaler {
	

public:
	
	enum ScaleModes {
		kScaleLinear,
		kScalePower2,
		kLPFilter
	}; 
	

	ScaleModes scaleMode;
	
	//int scaleMode;
	float linearScaleValue;
	float filterTerms;
	float accumulator;
	
	FlexibleScaler();
	FlexibleScaler(ScaleModes mode, float scale);
	
	float Scale(float input);
	void SetModeAndScaler(ScaleModes mode, float scale);
	
	
};

