#ifndef MRANDOM_H
#define MRANDOM_H

#include "mmathdriver.h"

// TODO: test this shit.
// TODO: implement crypto rng

/**
 * Random number generator interface.
 */
class MRandom
{
public:
	inline virtual ui32 Sample()
	{
		byte k[4];
		k[0] = MathDriver::Random() % 256;
		k[1] = MathDriver::Random() % 256;
		k[2] = MathDriver::Random() % 256;
		k[3] = MathDriver::Random() % 256;
		return *(ui32*)k;
	}

	inline virtual ui32 Next()
	{
		return Sample();
	}
	
	inline virtual void NextBytes( byte* dst, ui32 length)
	{
		while(length--)
		{
			*(dst++) = Sample() % 256;
		}
	}

	inline virtual ui32 Next(ui32 maxValue)
	{
		ui32 rnd = Sample();
		if (rnd > maxValue) return (rnd % maxValue);
		return rnd;
	}
	
	inline virtual ui32 Next(ui32 minValue, ui32 maxValue)
	{
		ui32 diff = maxValue - minValue;
		ui32 rnd = Sample() % diff;
		return rnd + minValue;
	}

	/**
	 * Generates a random between 0.0 and 1.0
	 */
	inline virtual float NextSingle()
	{
		return (float)((double)Sample() / (double)MAX_UI32);
	}
};

#endif