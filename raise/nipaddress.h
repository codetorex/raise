#ifndef NIPADDRESS_H
#define NIPADDRESS_H

#include "tstring.h"
#include "texception.h"
#include "tconvert.h"
#include "tstringformat.h"

#ifdef LINUX
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

class NIPAddress
{
public:
	union
	{
		struct  
		{
			byte a1;
			byte a2;
			byte a3;
			byte a4;
		};
		byte Parts[4];
		ui32 Address;
	};

	NIPAddress()
	{
		Address = 0; // 0.0.0.0
	}

	NIPAddress(ui32 value)
	{
		Address = value;
	}

	NIPAddress(const TString& value)
	{
		Address = inet_addr((char*)value.Data); //linux have this function too so no need for re writing it ?
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

	TString ToString()
	{
		return TString::Format("%.%.%.%",sfu(a1),sfu(a2),sfu(a3),sfu(a4));
	}
};


enum NProtocol
{
	NP_TCP,
	NP_UDP,
};


#endif