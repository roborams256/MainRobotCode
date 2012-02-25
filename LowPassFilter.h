

class LowPassFilter{
	
public:
	LowPassFilter(int terms);
	~LowPassFilter(void);
	
	float *samples;
	int numTerms;
	int headPointer;
	float Get(float input);
};
