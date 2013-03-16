#ifndef TSOUND_H
#define TSOUND_H

#include "tarray.h"

class TSoundOscillator
{
public:
	float Frequency;
	float Period;
	float CurrentTime;

	TSoundOscillator(float freq)
	{
		SetFrequency(freq);
		CurrentTime = 0.0f;
	}

	void SetFrequency(float newFrequency)
	{
		Frequency = newFrequency;
		Period = 1.0f / newFrequency;
	}

	virtual float Generate(float timeStep) = 0;
};

class TSoundOscillatorSinus: public TSoundOscillator
{
public:
	float Generate( float timeStep ) 
	{
		
	}
};

class TSoundChannel
{
public:

};

/**
 * Special kind of music generator.
 */
class TSoundTracker
{
public:
	TArray< TSoundChannel* > Channels; 
};

class TSound
{
public:	
	

};


#endif