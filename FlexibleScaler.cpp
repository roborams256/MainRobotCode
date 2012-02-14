#import "FlexibleScaler.h"


FlexibleScaler::FlexibleScaler(){
	
	scaleMode = kScaleUnity;
	linearScaleValue = 0.5;
};

FlexibleScaler::FlexibleScaler(ScaleModes mode, float scale){
	
	scaleMode = mode;
	linearScaleValue = scale;
	
}

float FlexibleScaler::Scale(float input){
	
	int sign;
	float rval;
	
	switch (scaleMode) {
	
	case kScaleUnity:  // unity function
		return input;
		break;
		
	case kScaleLinear:
		
		return input*linearScaleValue;
		
		
	case kScalePower2:
		sign = (input < 0) ? -1 : 1;
		
		rval = sign * (input * input * 100 ) / 100.0;
		
		return rval;
		break;
		
	case kScalePower2AndLinear:
		sign = (input < 0) ? -1 : 1;
		
		rval = sign * (input * input * 100 ) / 100.0;
		
		return rval*linearScaleValue;
		break;
		
		

		
	default:
		return input;
		
		
	}
	
}

void FlexibleScaler::SetModeAndScaler(ScaleModes mode, float scale){
	
	;
}
