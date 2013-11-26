#ifndef TEST_STRING_H
#define TEST_STRING_H


#include "ttest.h"
#include "tstring.h"

class TestTString: public UnitTest
{
public:
	TestTString(): UnitTest("TString check"){}

	void Execute()
	{
		String str_test = "Hello";

		Result( str_test == "Hello" ,"String creation from char* and equality test");
		Result( str_test.Length == 5, "String length test");
		Result( str_test.ByteLength == 5, "String byte length test");
		Result( str_test.Capacity == 0, "Const char* strings are doesn't have capacity");
		Result( str_test.Ref == 0, "Const char* string doesnt have reference");

		str_test += ", World!";

		Result(str_test == "Hello, World!", "String equality and append test");
		Result(str_test.Length == 13, "Appended string length test" );
		Result(str_test.ByteLength == 13, "Appended string byte length test");
		Result(str_test.Ref != 0, "Appended strings should have a reference");
		Result(str_test.Capacity == str_test.Ref->Capacity, "Appended string ref capacity equality");

		Result( str_test.StartsWith("Hello") , "StartsWith function test");
		Result( !str_test.StartsWith("!hello"), "Starts with function test 2");

		String test2 = str_test;
		Result(test2.Ref == str_test.Ref, "String re assign reference equality test");
		Result(test2.Data == str_test.Data, "String re assign data equality test");
		Result(test2.Length == str_test.Length, "String re assign length test");
		Result(test2.ByteLength == str_test.ByteLength, "String re assign byte length test");
		
		test2 += "Welcome Back!";

		Result(test2.Ref != str_test.Ref, "String append reference equality test");
		Result(test2 == "Hello, World!Welcome Back!", "String append equality test");
		Result(str_test == "Hello, World!", "String append test");
		Result(test2.Length == 26 && test2.ByteLength == 26, "String append length test");

		test2.Truncate(5);
		Result(test2 == "Hello", "String truncate test");

		int indice = test2.IndexOf("l");
		Result(indice == 2, "String index of test");
		Result(test2.EndsWith("lo"),"String ends with test");
		indice = test2.IndexOf("z");
		Result(indice == -1, "String index of not found test");

		// TODO: test other index of functions because they are more complex!

		String sub = str_test.Substring(6);
		Result(str_test.Substring(1) == "ello, World!","String sub string test 1");
		Result(str_test.Substring(0) == "Hello, World!","String sub string test 2");
		Result(sub == " World!", "String sub string test 3");
		sub = str_test.Substring(7,2);
		Result(sub == "Wo", "String sub string test 4");

		Result( str_test.GetFirst() == 'H', "String get first char test");
		Result( str_test.GetLast() == '!', "String get last char test");

		sub = "bus";
		Result(sub == "bus", "String re assign test");

		sub = "Bus" + str_test;
		Result(sub == "BusHello, World!", "String re assign and add check");

		sub = "   sub   ";
		Result(sub.TrimStart() == "sub   ", "String trim start");
		Result(sub.TrimEnd() == "   sub", "String trim end");
		Result(sub.Trim() == "sub", "String trim");
		Result(sub.Trim().TrimEnd() == "sub", "String trim and trim end");
		

		sub = "   sub   ";
		sub.TrimStartInplace();
		Result(sub == "sub   ", "String trim start (inplace)");
		Result(sub.Length == 6, "String trim start length (inplace)");
		Result(sub.ByteLength == 6, "String trim start byte length (inplace)");

		sub = "   sub   ";
		sub.TrimEndInplace();
		Result(sub == "   sub", "String trim end (inplace)");
		Result(sub.Length == 6, "String trim end length (inplace)");
		Result(sub.ByteLength == 6, "String trim end byte length (inplace)");

		sub = "   sub   ";
		sub.TrimInplace();
		Result(sub == "sub", "String trim (inplace)");
		Result(sub.Length == 3, "String trim length (inplace)");
		Result(sub.ByteLength == 3, "String trim byte length (inplace)");

		String splittest = "01/12/2011";
		Array<ch32> splitchars;
		splitchars.Add('/');

#ifdef WIN32
		// TODO: fix bugs in here


		Array<String*> SplitResult = splittest.Split( splitchars);
		Result(SplitResult.Count == 3, "String split result count");
		Result(*SplitResult.Items[0] == "01", "String split first element");
		Result(*SplitResult.Items[1] == "12", "String split second element");
		Result(*SplitResult.Items[2] == "2011", "String split third element");

		splitchars.Add('\\');

		splittest = "01\\12/2011";
		SplitResult = splittest.Split( splitchars );
		Result(SplitResult.Count == 3, "String split multi result count");
		Result(*SplitResult.Items[0] == "01", "String split multi first element");
		Result(*SplitResult.Items[1] == "12", "String split multi second element");
		Result(*SplitResult.Items[2] == "2011", "String split multi third element");

		splittest = "01//2011";
		SplitResult = splittest.Split( splitchars );
		Result(SplitResult.Count == 3, "String split multi result count, not remove empty");
		Result(*SplitResult.Items[0] == "01", "String split multi first element, not remove empty");
		Result(*SplitResult.Items[1] == String::Empty, "String split multi second element, not remove empty");
		Result(*SplitResult.Items[2] == "2011", "String split multi third element, not remove empty");

		splittest = "01//2011";
		SplitResult = splittest.Split( splitchars, true );
		Result(SplitResult.Count == 2, "String split multi result count, remove empty");
		Result(*SplitResult.Items[0] == "01", "String split multi first element, remove empty");
		Result(*SplitResult.Items[1] == "2011", "String split multi second element, remove empty");
#endif // WIN32

		String fmttest = String::Format("Test % % %", sfi(5), sfi(4), sfi(3));
		Result(fmttest == "Test 5 4 3", "String basic format test");

		fmttest = String::Format("Test % % %", sfi(5,2,'0'),sfi(4,2,'0'),sfi(3,3,'0'));
		Result(fmttest == "Test 05 04 003", "String padded basic format test");

		fmttest = String::Format("Test %", sfx(64));
		Result(fmttest == "Test 40", "String format hex test");

		// TODO: test unicode stuff here
	}

} TStringCheck;

#endif