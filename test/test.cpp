// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "toldtests.h"



#ifdef WIN32
int _tmain(int argc, wchar_t* argv[])
#else
int main(int argc,char** argv)
#endif
{
	TBitmapStackTest s;
	s.Test();

	/*int k = 1;
	for (int i=0;i<32;i++)
	{
		k = calcval(i-1);

		printf("%12i, ",k);
		if ( (i+1) % 4 == 0)
		{
			printf("\n");	
		}
	}*/



	getchar();
	return 0;
}

