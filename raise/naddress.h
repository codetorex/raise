#ifndef NADDRESS_H
#define NADDRESS_H

#include "tstring.h"

#ifdef LINUX
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

/**
 * TODO: make this. Address that supports ipV4 or ipV6
 */
class NAddress
{
public:

};

class NAddress4
{
public:
	union
	{
		struct  
		{
			byte A;
			byte B;
			byte C;
			byte D;
		};
		byte Parts[4];
		ui32 Address;
	};


	inline void Set( const TString& value)
	{
		Address = Parse(value);
	}

	inline void Set( ui32 value )
	{
		Address = value;
	}

	inline void Set(byte partA, byte partB, byte partC, byte partD)
	{
		A = partA;
		B = partB;
		C = partC;
		D = partD;
	}

	inline NAddress4()
	{
		Address = 0; // 0.0.0.0
	}

	inline NAddress4(ui32 value)
	{
		Set(value);
	}

	inline NAddress4(const TString& value)
	{
		Set(value);
	}

	inline NAddress4( const char* value )
	{
		TString tp = value;
		Set(tp);
	}

	inline NAddress4(byte partA, byte partB, byte partC, byte partD)
	{
		Set(partA,partB,partC,partD);
	}

	/**
	 * Parses given string to ip address. NADDREESS& already does this?
	 */
	/*inline NAddress4& operator = ( const TString& value  )
	{
		Set(value);
		return *this;
	}*/

	inline NAddress4& operator = ( ui32 value )
	{
		Set(value);
		return *this;
	}

	inline NAddress4& operator = ( const NAddress4& value )
	{
		Address = value.Address;
		return *this;
	}

	static ui32 SystemParse(const TString& value)
	{
		if (!value.IsASCII())
		{
			throw Exception("Unicode in ip address");
		}
		return inet_addr((char*)value.Data);
	}

	static ui32 Parse(const TString& value)
	{
		if (!value.IsASCII())
		{
			throw Exception("Unicode in ip address");
		}
		char tmp[128];
		char* sparts[4];
		int spartl[4];

		MemoryDriver::Copy(tmp,value.Data,value.ByteLength);
		tmp[value.ByteLength] = 0;

		sparts[0] = tmp;
		int curPart = 0;
		int curPartLength = 0;

		for (ui32 i=0;i<value.ByteLength;i++)
		{
			if (tmp[i] == '.')
			{
				if (curPartLength == 0 || curPartLength > 3)
				{
					throw Exception("Improper ip address");
				}
				spartl[curPart] = curPartLength;
				tmp[i] = 0;
				curPart++;
				sparts[curPart] = &tmp[i+1];
				curPartLength = 0;
			}
			else
			{
				curPartLength++;
			}
		}

		if (curPart != 3 || curPartLength == 0 || curPartLength > 3)
		{
			throw Exception("Improper ip address");
		}

		int ai = Convert::ToInt32(sparts[0]);
		int bi = Convert::ToInt32(sparts[1]);
		int ci = Convert::ToInt32(sparts[2]);
		int di = Convert::ToInt32(sparts[3]);

		if ((ai > 255 || ai < 0) || (bi > 255 || bi < 0)|| (ci > 255 || ci < 0)|| (di > 255 || di < 0))
		{
			throw Exception("Improper ip address");
		}

		byte resultbytes[4];
		resultbytes[0] = ai;
		resultbytes[1] = bi;
		resultbytes[2] = ci;
		resultbytes[3] = di;

		ui32 result = *(ui32*)resultbytes;

		return result;
	}

	inline TString ToString()
	{
		return TString::Format("%.%.%.%",sfu(A),sfu(B),sfu(C),sfu(D));
	}
};

class NEndPoint
{
public:
	NAddress4	Address;
	ui16		Port;

	inline NEndPoint()
	{
		Address = 0; // AKA ANY
		Port = 0; // AKA ANY
	}

	inline NEndPoint(const NAddress4& _address, ui16 _port)
	{
		Set(_address,_port);
	}

	inline void Set(const NAddress4& _address, ui16 _port)
	{
		Address = _address;
		Port = _port;
	}

	inline NEndPoint& operator = ( const NEndPoint& value )
	{
		Address = value.Address;
		Port = value.Port;
		return *this;
	}
};


#endif