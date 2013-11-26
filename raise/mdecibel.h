#ifndef MDECIBEL_H
#define MDECIBEL_H

class MDecibel
{
private:
	static const float LOG2_DB = 8.6858896380650365530225783783321;
	static const float DB_2LOG = 0.11512925464970228420089957273422;

public:
	static float LinearToDecibel(float lin)
	{
		return Math.Log(lin) * LOG2_DB;
	}

	static float DecibelToLinear(float db)
	{
		return Math.Exp(db * DB_2LOG);
	}
};

#endif