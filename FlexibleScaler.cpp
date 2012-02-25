#import "FlexibleScaler.h"


FlexibleScaler::FlexibleScaler(){
	
	scaleMode = kScaleLinear;
	linearScaleValue = 0.85;
	filterTerms = 10.0;
	accumulator = 0;
};

FlexibleScaler::FlexibleScaler(ScaleModes mode, float scale){
	
	scaleMode = mode;
	linearScaleValue = scale;
	filterTerms = 10.0;
	accumulator = 0;
	
	
}

float FlexibleScaler::Scale(float input){
	
	float sign;
	float rval;
	
	switch (scaleMode) {
	
		
	case kScaleLinear:
		
		rval = input*linearScaleValue;
		break;
		
		
	case kScalePower2:
		sign = (input < 0) ? -1.0 : 1.0;
		
		rval = sign * (input * input );
		
		return rval*linearScaleValue;
		break;
		
	case kLPFilter:
		
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
