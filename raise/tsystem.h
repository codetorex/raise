#ifndef TSYSTEM_H
#define TSYSTEM_H

class TSystem
{
public:
	static void ShowMessage(const char* fmt,...)
	{
		char Temp[4096];

		va_list ap;
		va_start(ap,fmt);
		vsprintf_s(Temp,4096,fmt,ap);
		va_end(ap);

		MessageBoxA(0,Temp,"Message",MB_OK);
	}
};

#endif