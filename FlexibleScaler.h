
class FlexibleScaler {
	

public:
	
	enum ScaleModes {
		kScaleUnity,
		kScaleLinear,
		kScalePower2,
		kScalePower2AndLinear
	}; 
	

	ScaleModes scaleMode;
	
	//int scaleMode;
	float linearScaleValue;
	
	FlexibleScaler();
	FlexibleScaler(ScaleModes mode, float scale);
	
	float Scale(float input);
	void SetModeAndScaler(ScaleModes mode, float scale);
	
	
};

