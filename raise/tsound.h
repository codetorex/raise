#ifndef TSOUND_H
#define TSOUND_H

#include "tarray.h"
#include "tstring.h"
#include <math.h>

namespace raise
{
	namespace Sound
	{
		class AudioFormat
		{
		public:
			ui32 SampleRate; // 44100
			ui32 BitsPerSample;
			ui32 Format;
		};

		class Sampler
		{
		public:
			AudioFormat* Format;
			virtual float GetSample(ui32 sampleIndex) = 0;

			float GetAverage(ui32 startSample, ui32 endSample)
			{
				float sampleSum = 0.0f;
				for (ui32 i=startSample;i<endSample;i++)
				{
					sampleSum += GetSample(i);
				}
				sampleSum /= (float)(endSample - startSample);
				return sampleSum;
			}
		};

		class Oscillator: public Sampler
		{
		public:
			
			ui32 Frequency;
			ui32 SamplePeriod;
			ui32 HalfSamplePeriod;

			Oscillator(AudioFormat* format, ui32 frequency)
			{
				this->Format = format;
				this->Frequency = frequency;
			}

			Oscillator()
			{
				Format = NULL;
				Frequency = 0;
			}

			void SetFrequency(ui32 hertz)
			{
				Frequency = hertz;
				SamplePeriod = Format->SampleRate / Frequency;
				HalfSamplePeriod = SamplePeriod / 2;
			}
		};

		class SinusOscillator: public Oscillator
		{
		public:
			float GetSample(ui32 sample)
			{
				ui32 curPos = sample % SamplePeriod;
				float fpos = (curPos / (float)SamplePeriod) * DOUBLEPI ;
				return sin(fpos);
			}
		};

		class SawtoothOscillator: public Oscillator
		{
		public:
			float GetSample(ui32 sample)
			{
				ui32 curPos = sample % SamplePeriod;
				float fpos = ((curPos / (float)SamplePeriod)* 2.0f) - 1.0f;
				return fpos;
			}
		};

		class InterestingOscillator: public Oscillator
		{
		public:
			float GetSample(ui32 sample)
			{
				ui32 curPos = sample % SamplePeriod;
				float fpos =((curPos / (float)SamplePeriod)* 2.0f) - 1.0f;
				if (curPos > HalfSamplePeriod)
				{
					fpos = 2.0f - fpos;
				}
				return fpos;
			}
		};

		class SquareOscillator: public Oscillator
		{
		public:
			float GetSample(ui32 sample)
			{
				ui32 curPos = sample % SamplePeriod;
				if (curPos > HalfSamplePeriod)
				{
					return -1.0f;
				}
				
				return 1.0f;
			}
		};

		class TriangleOscillator: public Oscillator
		{
		public:
			float GetSample(ui32 sample)
			{
				ui32 curPos = sample % SamplePeriod;
				float fpos =((curPos / (float)SamplePeriod)* 2.0f);
				if (curPos > HalfSamplePeriod)
				{
					fpos = 2.0f - fpos;
				}
				fpos = fpos - 0.5f;
				return fpos;
			}
		};
	}
}



class TSoundWave
{
public:
	float Frequency;
	float Period;
	float CurrentTime;

	TSoundWave(float freq)
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

class TSoundWaveSinus: public TSoundWave
{
public:
	float Generate( float timeStep ) 
	{
		float curpart = fmod(timeStep,Period);
		curpart /= Period;
		curpart *= DOUBLEPI;
		return sin(curpart);
	}
};

class TSoundWaveVisualizer
{
public:
	
};

class TSoundChannel
{
public:
	String Name;

	/// Sample offset of channel data
	ui32 Offset;

	virtual float GetValue(float timeStep) = 0;
};



class TSound
{
public:	
	enum SoundFormats
	{
		SF_16BIT,
		SF_32BIT,
		SF_FLOAT,
	};
	
	ui32 BitsPerSample;
	ui32 SampleRate;
	SoundFormats Format;

	/// Like left channel, right channel
	Array< TSoundChannel* > Channels;
};

class TSoundPlayer
{
public:
	void Play(TSound* snd);
};

class TTrackerChannel: public TSoundChannel
{
public:

};

/**
 * Special kind of music generator.
 */
class TTracker: public TSound
{
public:
	Array< TTrackerChannel* > Tracks;

};



#endif