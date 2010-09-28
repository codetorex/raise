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
		printf("Our func: %u allocations of 124 bytes in %u msecs\n", tp.callCount, tp.tickDelta);

		tp.BeginProfiling(131072);
		for (int i=0;i<131072;i++)
		{
			pok = (byte*) new byte[128];
		}
		tp.EndProfiling();
		printf("Ms func: %u allocations of 124 bytes in %u msecs\n", tp.callCount,tp.tickDelta);

	}
};

class TStringTest
{
public:

	void PrintString(string& k)
	{
		wprintf(k.Chars);
		wprintf(L" %i %i\n", k.Length, k.Capacity);
	}

	void Test()
	{
		string teststr = L"this is test string";
		PrintString(teststr);
		teststr.ToUpper();
		PrintString(teststr);
		teststr.ToLower();
		PrintString(teststr);

		string pok = teststr.Substring(0,4);
		PrintString(pok);


		string test2 = L"Concentration test: ";
		PrintString(test2);
		test2 += L"Concentration string";
		PrintString(test2);
		test2 += pok + pok;
		PrintString(test2);
		test2 += L" ";
		PrintString(test2);
		str8 eight = " 8bit string ";
		test2 += eight;
		PrintString(test2);
		test2 += 8;
		PrintString(test2);
		test2 += L" -> ";
		PrintString(test2);
		test2 += 6.0f;
		PrintString(test2);

		if (test2 == L"Concentration test: Concentration stringthisthis  8bit string 8 -> 6.00")
		{
			wprintf(L"IT WORKSSSS\n");
		}
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

