#include "LowPassFilter.h"



LowPassFilter::LowPassFilter(int terms){
	
	//create a new array the lenght of terms
	samples = new float[terms]; 
	headPointer = 0;
	numTerms = terms;
	
	//clear it
	
	for (int i=0; i< terms; i++){
		samples[i] = 0;
	}
		
}

LowPassFilter::~LowPassFilter(void){
	
	delete [] samples;
}

float LowPassFilter::Get(float input){
	
	samples[headPointer] = input;
	
	if (headPointer++>(numTerms-1))
		headPointer = 0; // wrap it
	
	float sum = 0;
	for (int i=0; i<numTerms; i++)
		sum+=samples[i];
	
	return sum/(float)numTerms;
	
}