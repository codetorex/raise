#ifndef TTEXTWRITER_H
#define TTEXTWRITER_H

#include "tstring.h"
#include "tencodings.h"

//TODO: make this class converts its content to UTF-8 to UTF-16

class TTextWriter
{
public:
	TEncoding& Encoding;

	TTextWriter(): Encoding( Encodings.UTF8 ) 
	{	
		set_NewLine("\r\n"); // windows standart
	};

	virtual void WriteChar(ch32 chr) = 0;
	
	inline virtual void WriteCharEnumerator(TCharacterEnumerator& enm)
	{
		while(enm.MoveNext())
		{
			WriteChar(enm.Current);
		}
	}

	// TODO: implement other functions like ones that writes the ui32 and float numbers and stuff like that as in .net framework one

	inline virtual void Write(const TString& value)
	{
		TCharacterEnumerator ce(value);
		WriteCharEnumerator(ce);
	}

	inline virtual void Write(const TStringBuilder& builder)
	{
		TCharacterEnumerator ce(builder);
		WriteCharEnumerator(ce);
	}

	inline virtual void Write(TArray<ch32>& CharArray)
	{
		TArrayEnumerator<ch32> ae(CharArray);
		while(ae.MoveNext())
		{
			WriteChar(ae.Current);
		}
	}

	inline void Write(const TString& fmt, sfp arg0)
	{
		Write(TString::Format(fmt,arg0));
	}

	inline void Write(const TString& fmt, sfp arg0, sfp arg1)
	{
		Write(TString::Format(fmt,arg0,arg1));
	}

	inline void Write(const TString& fmt, sfp arg0, sfp arg1, sfp arg2)
	{
		Write(TString::Format(fmt,arg0,arg1,arg2));
	}

	inline void Write(const TString& fmt, sfp arg0, sfp arg1, sfp arg2,sfp arg3)
	{
		Write(TString::Format(fmt,arg0,arg1,arg2,arg3));
	}

	inline void WriteLine(const TString& fmt, sfp arg0)
	{
		WriteLine(TString::Format(fmt,arg0));
	}

	inline void WriteLine(const TString& fmt, sfp arg0, sfp arg1)
	{
		WriteLine(TString::Format(fmt,arg0,arg1));
	}

	inline void WriteLine(const TString& fmt, sfp arg0, sfp arg1, sfp arg2)
	{
		WriteLine(TString::Format(fmt,arg0,arg1,arg2));
	}

	inline void WriteLine(const TString& fmt, sfp arg0, sfp arg1, sfp arg2,sfp arg3)
	{
		WriteLine(TString::Format(fmt,arg0,arg1,arg2,arg3));
	}

	inline virtual void WriteLine()
	{
		for (int i=0;i<CoreNewLineLength;i++)
		{
			WriteChar(CoreNewLine[i]);
		}
	}

	inline void RepeatChar(ch32 value, ui32 amount = 50)
	{
		for (ui32 i=0;i<amount;i++)
		{
			WriteChar(value);
		}
	}

	inline virtual void WriteLine(const TString& value)
	{
		Write(value);
		WriteLine();
	}


	inline virtual TString& get_NewLine()
	{
		return NewLine;
	}

	inline virtual void set_NewLine(const TString& value)
	{
		NewLine = value;
		TCharacterEnumerator ce(value);
		while(ce.MoveNext())
		{
			CoreNewLine[ce.CharIndex] = ce.Current;
			CoreNewLineLength = ce.CharIndex+1;
		}
	}

	inline virtual void Close()
	{
		
	}

protected:
	TString NewLine;
	ch32 CoreNewLine[16];
	int CoreNewLineLength;
};



#endif