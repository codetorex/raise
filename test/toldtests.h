
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
#include "tbitfield.h"
#include "tbitstack.h"

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
		bmp.create(128,128, FRGB ); 

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