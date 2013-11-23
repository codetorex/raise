#ifndef TCONSOLEVT100_H
#define TCONSOLEVT100_H

#include "tconsolebase.h"
#include "tstream.h"

class TConsoleVT100: public TConsole // CAN BE USED FOR WRITING APPLICATIONS
{
public:

	TStream* Output;
	TStream* Input;

	bool UseXTermExtensions;

	TConsoleVT100()
	{

	}

	TConsoleVT100(TStream* out, TStream* inp)
	{
		Initialize(out,inp);
	}

	inline void Initialize()
	{
		Initialize(new TFileStream(stdout,"STANDART OUTPUT", fm_Write), new TFileStream(stdin,"STANDART INPUT", fm_Read));
	}

	inline void Initialize(TStream* out,TStream* inp)
	{
		this->Output = out;
		this->Input = inp;
		UseXTermExtensions = true;
		set_NewLine("\n");
	}

	// CREATES ESCAPE SEQUENCES

	void GetSize() 
	{
		this->Size.SetSize(80,24);
	}

	void GetPosition() 
	{
		//Write("\x1B6n"); // must read after this
		// TODO: read position
	}

	void SetPosition( IPosition& pos ) 
	{
		Write("\x1B[%;%H",sfi(pos.X),sfi(pos.Y));
	}

	void SetBackColor( byte color ) 
	{
		if (UseXTermExtensions)
		{
			this->Write("\x1B[48;5;%m", sfi(color));
		}
	}

	void SetForeColor( byte color ) 
	{
		if (UseXTermExtensions)
		{
			this->Write("\x1B[38;5;%m", sfi(color));
		}
	}

	void Clear() 
	{
		Write("\x1B[2J");
	}

	inline ch32 ReadKey()
	{
		byte tmp[4];
		Input->Read(tmp,1,1);
		return tmp[0];
	}

	/*inline void Write(const TString& value)
	{
		Output->Write(value.Data,1,value.ByteLength);
	}*/

	inline void WriteChar( ch32 chr ) 
	{
		byte tmp[8];
		int l = StringDriver::Encode(tmp,chr);
		Output->Write(tmp,1,l);
	}

};


#endif