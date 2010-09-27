// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "tbitset.h"
#include "tmemory.h"
#include "tprofiler.h"
#include "tstring.h"

class TBitArrayTest
{
public:
	TBitset* bitArray;

	void Test()
	{
		bitArray = new TBitset(128); // 16 kbts
		
		bitArray->FillBits(5,11,true);
		bitArray->FillBits(18,31,true);
		printf("%s\n",bitArray->GetBitsAsChar(0,64));

		int emptyBits4 = bitArray->SearchEmptyBits(4,0);
		printf("First empty 4 bits = %i\n",emptyBits4);

		int emptyBits32 = bitArray->SearchEmptyBits(32,0);
		printf("First empty 32 bits = %i\n",emptyBits32);
		
		bitArray->FillBits(emptyBits32,32,true);
		bitArray->FillBits(127,1,true);
		printf("%s\n",bitArray->GetBitsAsChar(0,128));

	}
};

class TMemoryTest
{
public:
	TMemoryManager* mm;

	void Test()
	{
		TProfiler tp;
		
		mm = new TMemoryManager();
		TMemoryPool* mp = mm->CreateNewPool(16 * MB);
		mp->UseBitmapAllocator(128);

		void* pok;

		tp.BeginProfiling(131072);
		for (int i=0;i<131072;i++)
		{
			pok = mp->Allocator->Allocate(124);
		}
		tp.EndProfiling();
		printf("Our func: %i allocations of 124 bytes in %i msecs\n", tp.callCount, tp.tickDelta);

		tp.BeginProfiling(131072);
		for (int i=0;i<131072;i++)
		{
			pok = (byte*) new byte[128];
		}
		tp.EndProfiling();
		printf("Ms func: %i allocations of 124 bytes in %i msecs\n", tp.callCount,tp.tickDelta);

	}
};

class TStringTest
{
public:

	void Test()
	{
		string kamil = L"Kamil";
		kamil += L" Solak\n";
		wprintf(kamil);


		string* testString = new string(L"Hello");
		*testString += L" ,world!\n";
		testString->ToUpper();
		wprintf(*testString);

		TString8 testString2;

		testString2 = *testString;
		testString2.ToLower();
		printf(testString2);

		string formatted = string::Format(L"Selam ben %i numarali vatandas %f yasindayim\n",4096, 12.555f);
		wprintf(formatted);

		string padding = L"12.5";
		padding.PadLeft(10);
		wprintf(padding);

		TString8 padding2 = "Kamil";
		padding2.PadLeft(10);
		printf(padding2);
	}
};

#ifdef WIN32
int _tmain(int argc, wchar_t* argv[])
#else
int main(int argc,char** argv)
#endif
{
	/*TBitArrayTest k;
	k.Test();*/

	/*TMemoryTest m;
	m.Test();*/

	TStringTest s;
	s.Test();


	getchar();
	return 0;
}

