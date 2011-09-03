
#include "tbitset.h"
#include "tmemory.h"
#include "tprofiler.h"
#include "tstring.h"
#include "mvector3.h"
#include "mquaternion.h"
#include "mray.h"
#include "mfixed.h"

#include "tcolor.h"
#include "tbitmap.h"
#include "tbinary.h"
#include "tbitfield.h"
#include "tbitstack.h"


#include "thashmap.h"
#include <hash_map>



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

class TStringTest
{
public:

	void PrintString(stringfw& k)
	{
		wprintf(k.Chars);
		wprintf(L" %i %i\n", k.Length, k.Capacity);
	}

	void Test()
	{
		stringfw teststr = L"this is test string";
		PrintString(teststr);
		teststr.ToUpper();
		PrintString(teststr);
		teststr.ToLower();
		PrintString(teststr);

		stringfw pok = teststr.Substring(0,4);
		PrintString(pok);


		stringfw test2 = L"Concentration test: ";
		PrintString(test2);
		test2 += L"Concentration string";
		PrintString(test2);
		test2 += pok + pok;
		PrintString(test2);
		test2 += L" ";
		PrintString(test2);
		TString eight = " 8bit string ";
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
		bmp.Create(128,128, TBitmapFormats::fRGB ); 

		TColor24 clr2(255,128,64);
		for (int y=0;y<128;y++)
		{
			for (int x=0;x<128;x++)
			{
				bmp.SetPixel(x,y,clr2.bclr);
			}
		}


	}
};


class TestBinary
{
public:
	void PrintK(TBinary& k)
	{
		printf("log2:\t%i\n",k.log2());
		printf("log2k:\t%i\n",k.log2k());
		printf("popcount:\t%i\n",k.popcount());
		printf("is2n:\t%i\n",k.is2n());
	}

	void Test()
	{
		TBinary k(1024);
		PrintK(k);
		k.value = 7543;
		PrintK(k);
		k.value = MXDWORD;
		PrintK(k);

	}
};

class TBitmapStackTest
{
public:

	void Report(TBitstack& bin,bool val)
	{
		printf("%s",bin.tostring().Chars);
		if (val)
		{
			printf("\tTRUE\n");
		}
		else
		{
			printf("\tFALSE\n");
		}
	}

	void Test()
	{
		TBitstack tb = 0;
		printf("%s\n",tb.tostring().Chars);
		tb.pushs(-7,4);printf("%s\n",tb.tostring().Chars);
		tb.push(1,4);printf("%s\n",tb.tostring().Chars);
		tb.pushs(-3,3);printf("%s\n",tb.tostring().Chars);
		tb.pushs(-2,3);printf("%s\n",tb.tostring().Chars);
		tb.pushs(-1,3);printf("%s\n",tb.tostring().Chars);
		tb.pushs(3,3);printf("%s\n",tb.tostring().Chars);

		Report(tb,tb.pops(3) == 3);
		Report(tb,tb.pops(3) == -1);
		Report(tb,tb.pops(3) == -2);
		Report(tb,tb.pops(3) == -3);
		Report(tb,tb.pop(4) == 1);
		Report(tb,tb.pops(4) == -7);
	}
};


using namespace std;

struct strCmp 
{
	static const int bucket_size = 32;

	dword operator() ( const TString& s1 ) const
	{
		return TString::GetHash(s1);
	}

	bool operator()( const TString& s1, const TString& s2 ) const
	{
		return (s1 == s2);
	}
};



class THashMapTest
{
public:
	TProfiler tp;

	void Test()
	{
		THashMap<int> myhashmap;
		myhashmap.Add(TString("quit"),500);

		if (myhashmap.GetValue(TString("quit")) == 500)
		{
			printf("IT WORKS!");
		}
	}

	void Test2()
	{
		vector<TString*> dataToHash;

		srand(65536);
		int i = 10000;
		while(i--)
		{
			TString* newstr = new TString(8);
			newstr->Length = 8;
			TString::Random(*newstr);
			dataToHash.push_back(newstr);
		}


		THashMap<int> myhashmap;

		i = 10000;
		tp.BeginProfiling(10000);
		while(i--)
		{
			myhashmap.Add(*dataToHash[i],rand());
		}
		tp.EndProfiling();
		printf("Your algorithm took %i ms for mapping 10000 entries.\n", tp.tickDelta);
		printf("Average %0.2f us per map\n\n",tp.PerOperationTime());


		int k = 100;
		tp.BeginProfiling(10000 * 100);
		while(k--)
		{
			i = 10000;
			while(i--)
			{
				int value = myhashmap.GetValue(*dataToHash[i]);
			}
		}
		tp.EndProfiling();
		printf("Your algorithm took %i ms for getting 10000000 entries.\n", tp.tickDelta);
		printf("Average %0.2f us per get\n\n",tp.PerOperationTime());


		i = 10000;
		hash_map<TString,int,strCmp> stdmap;
		tp.BeginProfiling(10000);
		while (i--)
		{
			stdmap[ *dataToHash[i]] = rand();
		}
		tp.EndProfiling();
		printf("STD algorithm took %i ms for mapping 10000 entries.\n", tp.tickDelta);
		printf("Average %0.2f us per map\n\n",tp.PerOperationTime());

		k = 100;
		tp.BeginProfiling(10000);
		while(k--)
		{
			i = 10000;
			while(i--)
			{
				int value = stdmap[*dataToHash[i]];
			}
		}
		tp.EndProfiling();
		printf("STD algorithm took %i ms for getting 10000000 entries.\n", tp.tickDelta);
		printf("Average %0.2f us per get\n\n",tp.PerOperationTime());

	}
};