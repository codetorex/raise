// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "toldtests.h"


#include "tdirectoryinfo.h"


#include "thashmap.h"
#include <hash_map>


using namespace std;

struct strCmp 
{
	static const int bucket_size = 32;

	dword operator() ( const str8& s1 ) const
	{
		return str8::GetHash(s1);
	}

	bool operator()( const str8& s1, const str8& s2 ) const
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
		myhashmap.Add(str8("quit"),500);

		if (myhashmap.GetValue(str8("quit")) == 500)
		{
			printf("IT WORKS!");
		}
	}

	void Test2()
	{
		vector<str8*> dataToHash;

		srand(65536);
		int i = 10000;
		while(i--)
		{
			str8* newstr = new str8(8);
			newstr->Length = 8;
			str8::Random(*newstr);
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
		hash_map<str8,int,strCmp> stdmap;
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


#ifdef WIN32
int _tmain(int argc, wchar_t* argv[])
#else
int main(int argc,char** argv)
#endif
{
	/*THashMapTest s;
	s.Test2();*/

	/*TBitmapStackTest s;
	s.Test();*/

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

