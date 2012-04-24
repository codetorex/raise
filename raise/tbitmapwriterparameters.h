#ifndef TBITMAPWRITERPARAMETERS_H
#define TBITMAPWRITERPARAMETERS_H

#include "tarray.h"

class TBitmapWriterParameter
{
public:
	ui32 ParameterID;

	union
	{
		float	FloatValue;
		i32		IntValue;
		ui32	UIntValue;
	};
};

class TBitmapWriterParameters
{
public:
	TArray<TBitmapWriterParameter> Parameters;

	inline void SetFloatParameter(ui32 ParameterID, float value)
	{
		TBitmapWriterParameter p;
		p.ParameterID = ParameterID;
		p.FloatValue = value;
		Parameters.Add(p);
	}

	inline void SetUInt32Parameter(ui32 ParameterID, ui32 value)
	{
		TBitmapWriterParameter p;
		p.ParameterID = ParameterID;
		p.UIntValue = value;
		Parameters.Add(p);
	}

	inline void SetInt32Parameter(ui32 ParameterID, i32 value)
	{
		TBitmapWriterParameter p;
		p.ParameterID = ParameterID;
		p.IntValue = value;
		Parameters.Add(p);
	}

	TBitmapWriterParameter* GetParameter(ui32 ParameterID)
	{
		for (ui32 i=0;i<Parameters.Count;i++)
		{
			TBitmapWriterParameter& curParam = Parameters.Item[i];
			if (curParam.ParameterID == ParameterID)
			{
				return &curParam;
			}
		}

		return 0;
	}

	inline bool GetInt32Parameter(ui32 ParameterID, int& value)
	{
		TBitmapWriterParameter* p = GetParameter(ParameterID);
		if (p == 0) return false;
		value = p->IntValue;
		return true;
	}

	inline bool GetUInt32Parameter(ui32 ParameterID, ui32& value)
	{
		TBitmapWriterParameter* p = GetParameter(ParameterID);
		if (p == 0) return false;
		value = p->UIntValue;
		return true;
	}

	inline bool GetFloatParameter(ui32 ParameterID, float& value)
	{
		TBitmapWriterParameter* p = GetParameter(ParameterID);
		if (p == 0) return false;
		value = p->FloatValue;
		return true;
	}
};

#endif