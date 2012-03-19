#ifndef NIPADDRESS_H
#define NIPADDRESS_H

#include "tstring.h"
#include "texception.h"

#ifdef LINUX
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

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
		*this = value;
	}

	inline void Set( ui32 value )
	{
		*this = value;
	}

	inline void Set(byte partA, byte partB, byte partC, byte partD)
	{
		A = partA;
		B = partB;
		C = partC;
		D = partD;
	}

	NAddress4()
	{
		Address = 0; // 0.0.0.0
	}

	NAddress4(ui32 value)
	{
		Set(value);
	}

	inline NAddress4& operator = ( ui32 value )
	{
		Address = value;
		return *this;
	}

	NAddress4(const TString& value)
	{
		Set(value);

		//Address = inet_addr((char*)value.Data); //linux have this function too so no need for re writing it ?
		// TODO: write scan function... and fix this shit
		/*int nStart = 0;
		int nLength = 0;
		a1 = (byte)Convert::ToInt32Ambiguous(value,nStart,&nLength); //val->ParseInt(nStart,&nLength);
		if (nLength == 0)
		{
			throw Exception("Wrong ip string");
		}
		nStart += nLength+1;
		a2 = (byte)Convert::ToInt32Ambiguous(value,nStart,&nLength); //val->ParseInt(nStart,&nLength);
		if (nLength == 0)
		{
			throw Exception("Wrong ip string");
		}
		nStart += nLength+1;
		a3 = (byte)Convert::ToInt32Ambiguous(value,nStart,&nLength); //val->ParseInt(nStart,&nLength);
		if (nLength == 0)
		{
			throw Exception("Wrong ip string");
		}
		nStart += nLength+1;
		a4 = (byte)Convert::ToInt32Ambiguous(value,nStart,&nLength); //val->ParseInt(nStart,&nLength);
		if (nLength == 0)
		{
			throw Exception("Wrong ip string");
		}*/
	}



	/**
	 * Parses given string to ip address.
	 */
	NAddress4& operator = ( const TString& value )
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

		A = ai;
		B = bi;
		C = ci;
		D = di;

		return *this;
	}

	TString ToString()
	{
		return TString::Format("%.%.%.%",sfu(A),sfu(B),sfu(C),sfu(D));
	}
};


enum NProtocol
{
	NP_TCP,
	NP_UDP,
};

class NEndPoint
{
public:
	NAddress4	Address;
	ui16		Port;
};


#endif