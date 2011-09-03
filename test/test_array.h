#ifndef TEST_ARRAY_H
#define TEST_ARRAY_H

#include "ttest.h"
#include "tarray.h"

class TestTArray: public TTestCheck
{
public:
	TestTArray(): TTestCheck("TArray check") {}

	void Test()
	{
		TArray<int> intarray;

		for(int i=0;i<8;i++)
		{
			intarray.Add(i);
		}

		AddResult( intarray.Count == 8 , "Array count" );
		AddResult( intarray.Capacity == 8, "Array capacity" );

		AddResult( intarray.Contains( 5 ), "Array contains" );
		AddResult( !intarray.Contains( 9 ), "Array not contains" );

		AddResult( intarray[4] == 4 , "Array index operator" );

		intarray.RemoveLast();

		AddResult( intarray.Count == 7 && intarray.Capacity == 8, "Array remove last");

		intarray.RemoveAt( 3 );
		AddResult( intarray[3] != 3 && intarray.Count == 6 , "Array remove at" );

		intarray.Remove(1);
		AddResult( intarray[1] != 3 && intarray.Count == 5, "Array remove item" );

		int idx = intarray.IndexOf(9);
		AddResult( idx == -1, "Array index of non item" );

		idx = intarray.IndexOf(0);
		AddResult( idx == 0, "Array index of item");

		intarray.Clear();
		AddResult( intarray.Count == 0, "Array clear" );

		int vals[] = {5,4,3,2,1,0};
		TArray<int> secondarray(vals);
		AddResult( secondarray.Count == 5, "Const array count");
		AddResult( secondarray.Capacity == 0, "Const array capacity");
		AddResult( secondarray.Item == vals, "Const array item");
	}
} TArrayCheck;

#endif