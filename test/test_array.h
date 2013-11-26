#ifndef TEST_ARRAY_H
#define TEST_ARRAY_H

#include "ttest.h"
#include "tarray.h"

class TestTArray: public UnitTest
{
public:
	TestTArray(): UnitTest("TArray check") {}

	void OutputArrayItems(const Array<int>& value)
	{
		TStringBuilderStack<512> sb;
		sb.Append("Array Count: ");
		sb.Append(value.Count);
		sb.Append(" , Items: ");

		for (ui32 i=0;i<value.Count;i++)
		{
			sb.Append(value.Items[i]);
			sb.AppendChar(' ');
			sb.AppendChar(',');
		}
		sb.RemoveLast(2);

		Output(sb.ToString());
	}

	void Execute()
	{
		Array<int> intarray;

		for(int i=0;i<8;i++)
		{
			intarray.Add(i);
		}

		Result( intarray.Count == 8 , "Array count" );
		Result( intarray.Capacity == 8, "Array capacity" );

		Result( intarray.Contains( 5 ), "Array contains" );
		Result( !intarray.Contains( 9 ), "Array not contains" );

		Result( intarray[4] == 4 , "Array index operator" );

		intarray.RemoveLast();

		Result( intarray.Count == 7 && intarray.Capacity == 8, "Array remove last");

		intarray.RemoveAt( 3 );
		Result( intarray[3] != 3 && intarray.Count == 6 , "Array remove at" );

		intarray.Remove(1);
		Result( intarray[1] != 3 && intarray.Count == 5, "Array remove item" );

		int idx = intarray.IndexOf(9);
		Result( idx == -1, "Array index of non item" );

		idx = intarray.IndexOf(0);
		Result( idx == 0, "Array index of item");

		intarray.Clear();
		Result( intarray.Count == 0, "Array clear" );

		intarray.Add(0);
		intarray.Add(2);
		intarray.Add(4);
		intarray.Insert(1,1);
		intarray.Insert(3,3);

		Result( intarray.Count == 5, "Insert count");
		for (int i=0;i<5;i++)
		{
			Result( intarray.Items[i] == i, "Insert test");
		}

		intarray.Insert(-1,0);
		Result( intarray.Count == 6, "Insert count 2");
		Result( intarray.Items[0] == -1 && intarray.Items[1] == 0, "Insert test 2");

		intarray.Insert(5,6);

		intarray.RemoveBetween(0,1);
		intarray.RemoveBetween(3,3);

		Result(intarray.Count == 3, "Remove between count");
		Result(intarray.Items[0] == 0 && intarray.Items[1] == 1 && intarray.Items[2] == 2, "Remove between items test");

		int vals[] = {5,4,3,2,1,0};
		Array<int> secondarray(vals);
		Result( secondarray.Count == 5, "Const array count");
		Result( secondarray.Capacity == 0, "Const array capacity");
		Result( secondarray.Items == vals, "Const array item");
	}
} TArrayCheck;

#endif