// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "tbitset.h"
#include "tmemory.h"
#include "tprofiler.h"
#include "tstring.h"
#include "mvector.h"
#include "mquaternion.h"
#include "mray.h"
#include "mfixed.h"

#include "tcolor.h"
#include "tbitmap.h"
#include "tbinary.h"


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


class TestVector
{
public:

	void Test()
	{
		wprintf(L"size of a vector: %i\n", sizeof(vec3));

		vec3 testvector(3.0f,2.0f,1.0f);
		wprintf(L"%f %f %f\n",testvector.v[0],testvector.v[1],testvector.v[2]);
		wprintf(L"%f %f %f\n",testvector.x,testvector.y,testvector.z);

	}
};

class TestColor
{
public:
	void Test()
	{
		TColor32 clr(255,128,64,32);
		wprintf(L"r:%i g:%i b:%i a:%i %X\n",clr.r,clr.g,clr.b,clr.a,clr.color);
		wprintf(L"r:%i g:%i b:%i a:%i\n",clr.bclr[0],clr.bclr[1],clr.bclr[2],clr.bclr[3]);
		 
		TColor24 clr2;

		wprintf(L"size of dword: %i\n", sizeof(dword));
		wprintf(L"size32: %i size24: %i sizeHSL: %i\n",sizeof(TColor32),sizeof(TColor24),sizeof(TColorHSL));

		TColorHSL hsl = clr;
		wprintf(L"h:%i s:%i l:%i\n",hsl.h,hsl.s,hsl.l);
	}
};

class TestBitmap
{
public:

	void Test()
	{
		TBitmap bmp;
		bmp.create(128,128,24, CRGB );
		
		TColor24 clr2(255,128,64);
		for (int y=0;y<128;y++)
		{
			for (int x=0;x<128;x++)
			{
				bmp.setpixel(x,y,clr2.bclr);
			}
		}


	}
};

#define StartProfile(x) BeginProfiling(x);\
						for (int i=0;i<x;i++)\
						{

#define EndProfile(x)	}\
						EndProfiling(x);\

#define TESTCOUNT		10000000



class TestBinary
{
public:
	TProfiler tp;

	void BeginProfiling(dword _callCount)
	{
		tp.BeginProfiling(_callCount);
	}

	void EndProfiling(char* _algname)
	{
		tp.EndProfiling();
		str8 slm(_algname);
		slm.PadRight(20);
		printf(slm.Chars);
		printf("%i ms\t%0.2f us\n",tp.tickDelta,tp.PerOperationTime());
	}

	void PrintK(TBinary& k)
	{
		printf("log2_lookup:\t%i\n",k.log2_lookup());
		printf("log2_lookup_ik:\t%i\n",k.log2_lookup_ik());
		printf("log2_naive:\t%i\n",k.log2_naive());
		printf("log2_calc:\t%i\n",k.log2_calc());
		printf("popcount_naive:\t%i\n",k.popcount_naive());
		printf("popcount_brian:\t%i\n",k.popcount_brian());
		printf("popcount_prl:\t%i\n",k.popcount_prl());
		printf("is2n:\t%i\n",k.is2n());
	}

	void PerformanceTest()
	{
		TBinary k(1024);

		StartProfile(10000000);
		k.value = rand();
		k.log2_lookup();
		EndProfile("log2_lookup");

		StartProfile(10000000);
		k.value = rand();
		k.log2_naive();
		EndProfile("log2_naive");

		StartProfile(10000000);
		k.value = rand();
		k.log2_calc();
		EndProfile("log2_calc");


		StartProfile(10000000);
		k.value = rand();
		k.log2_lookup_ik();
		EndProfile("log2_lookup_ik");

		StartProfile(10000000);
		k.value = rand();
		k.popcount_naive();
		EndProfile("popcount_naive");

		StartProfile(10000000);
		k.value = rand();
		k.popcount_brian();
		EndProfile("popcount_brian");

		StartProfile(10000000);
		k.value = rand();
		k.popcount_prl();
		EndProfile("popcount_prl");

		StartProfile(10000000);
		k.value = rand();
		k.popcount_lookup();
		EndProfile("popcount_lookup");
	}

	void Test()
	{
		TBinary k(1024);
		PrintK(k);
		k.value = 7543;
		PrintK(k);
		k.value = ULONG_MAX;
		PrintK(k);

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

	/*TStringTest s;
	s.Test();*/

	/*TestVector s;
	s.Test();*/

	/*TestColor s;
	s.Test();*/

	/*TestBitmap s;
	s.Test();*/

	TestBinary s;
	s.PerformanceTest();
	s.Test();

	getchar();
	return 0;
}

