#ifndef THEXDUMP_H
#define THEXDUMP_H

#include "tstringwriter.h"

class THexDump
{
private:
	TStringWriter sw;

public:
	/// Source
	byte* Data;

	/// Source Length
	ui32 Length;

	/// Used for generating addresses
	ui32 LogicalAddress;

	/// Number of columns
	ui32 Columns;

	String LineStart;

	bool WriteAddress;

	bool HexAddress;

	bool HexData;

	bool WriteTextPart;

	TTextWriter* Target;

	inline void Reset()
	{
		LogicalAddress = 0;
		Columns = 16;
		WriteAddress = true;
		HexAddress = true;
		HexData = true;
		WriteTextPart = true;
	}

	inline void Initialize(byte* _data, ui32 _length, TTextWriter* output = 0)
	{
		Reset();
		Data = _data;
		Length = _length;
		if (output)
		{
			Target = output;
		}
		else
		{
			Target = &sw;
		}
	}

	THexDump()
	{
		Reset();
	}

	THexDump( byte* _data , ui32 _length )
	{
		Initialize(_data, _length);
	}

	THexDump(Array<byte>& barry)
	{
		Initialize(barry.Items,barry.Count);
	}

	void Dump()
	{
		ui32 CurLogicalAddress;
		for (ui32 i = 0; i< Length; i += Columns)
		{
			CurLogicalAddress = LogicalAddress + i;
			Target->Write(LineStart);

			if (WriteAddress)
			{
				if (HexAddress)
				{
					Target->Write("0x%",sfx(CurLogicalAddress,8,'0'));
				}
				else
				{
					Target->Write("%", sfu(CurLogicalAddress,10,'0'));
				}
			}

			Target->WriteChar('\t');

			for (ui32 k = 0; k < Columns;k++)
			{
				if ( i + k >= Length)
				{
					Target->RepeatChar(' ', HexData ? 3 : 4);
					continue;
				}
				
				byte curData = *(Data + i + k);
				if (HexData)
				{
					Target->Write("%",sfx( curData ,2,'0'));
				}
				else
				{
					Target->Write("%", sfu( curData,3,'0'));
				}
				Target->WriteChar(' ');

			}
			Target->WriteChar('\t');

			if (!WriteTextPart)
			{
				Target->WriteLine();
				continue;
			}

			for (ui32 k=0; k < Columns ; k++)
			{
				if (i + k >= Length)
				{
					Target->WriteChar(' ');
					continue;
				}
				
				byte curData = *(Data + i + k);
				if (curData < 32 || curData > 126)
				{
					curData = '.';
				}
				Target->WriteChar(curData);
			}
			Target->WriteLine();
		}
	}

	inline String ToString()
	{
		if (Target == &sw)
		{
			return sw.ToString();
		}

		return "String written to target, can't be retrieved";
	}


};

#endif