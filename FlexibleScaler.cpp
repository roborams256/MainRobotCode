#import "FlexibleScaler.h"


FlexibleScaler::FlexibleScaler(){
	
	scaleMode = kScaleLinear;
	linearScaleValue = 0.85;
	InitFilterQueue();
	
	
}

FlexibleScaler::FlexibleScaler(ScaleModes mode, float scale){
	
	scaleMode = mode;
	linearScaleValue = scale;
	InitFilterQueue();
	
}

void FlexibleScaler::InitFilterQueue(){
	
	filterLength = LP_FILTER_TERMS;
	
	for (int i=0; i<filterLength; i++){
		filterQueue.push_front(0.0);
	}
	
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
		
		filterQueue.push_front(input);
		filterQueue.pop_back(); // get rid of the oldest
		
		rval = 0;
		
		for (int i=0; i<filterLength; i++)
			rval = rval + filterQueue[i];
		
		return rval/filterLength;
		
		break;
		
		
		
	default:
		return input;
		
		
	}
	
}

void FlexibleScaler::SetModeAndScaler(ScaleModes mode, float scale){
	
	;
}
