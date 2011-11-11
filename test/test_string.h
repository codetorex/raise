#ifndef TEST_STRING_H
#define TEST_STRING_H


#include "ttest.h"
#include "tstring.h"

class TestTString: public TTestCheck
{
public:
	TestTString(): TTestCheck("TString check"){}

	void Test()
	{
		string str_test = "Hello";

		AddResult( str_test == "Hello" ,"String creation from char* and equality test");
		AddResult( str_test.Length == 5, "String length test");
		AddResult( str_test.ByteLength == 5, "String byte length test");
		AddResult( str_test.Capacity == 0, "Const char* strings are doesn't have capacity");
		AddResult( str_test.Ref == 0, "Const char* string doesnt have reference");

		str_test += ", World!";

		AddResult(str_test == "Hello, World!", "String equality and append test");
		AddResult(str_test.Length == 13, "Appended string length test" );
		AddResult(str_test.ByteLength == 13, "Appended string byte length test");
		AddResult(str_test.Ref != 0, "Appended strings should have a reference");
		AddResult(str_test.Capacity == str_test.Ref->Capacity, "Appended string ref capacity equality");

		AddResult( str_test.StartsWith("Hello") , "StartsWith function test");
		AddResult( !str_test.StartsWith("!hello"), "Starts with function test 2");

		string test2 = str_test;
		AddResult(test2.Ref == str_test.Ref, "String re assign reference equality test");
		AddResult(test2.Data == str_test.Data, "String re assign data equality test");
		AddResult(test2.Length == str_test.Length, "String re assign length test");
		AddResult(test2.ByteLength == str_test.ByteLength, "String re assign byte length test");
		
		test2 += "Welcome Back!";

		AddResult(test2.Ref != str_test.Ref, "String append reference equality test");
		AddResult(test2 == "Hello, World!Welcome Back!", "String append equality test");
		AddResult(str_test == "Hello, World!", "String append test");
		AddResult(test2.Length == 26 && test2.ByteLength == 26, "String append length test");

		test2.Truncate(5);
		AddResult(test2 == "Hello", "String truncate test");

		int indice = test2.IndexOf("l");
		AddResult(indice == 2, "String index of test");
		AddResult(test2.EndsWith("lo"),"String ends with test");
		indice = test2.IndexOf("z");
		AddResult(indice == -1, "String index of not found test");

		string sub = str_test.Substring(6);
		AddResult(str_test.Substring(1) == "ello, World!","String sub string test 1");
		AddResult(str_test.Substring(0) == "Hello, World!","String sub string test 2");
		AddResult(sub == " World!", "String sub string test 3");
		sub = str_test.Substring(7,2);
		AddResult(sub == "Wo", "String sub string test 4");

		AddResult( str_test.GetFirst() == 'H', "String get first char test");
		AddResult( str_test.GetLast() == '!', "String get last char test");

		sub = "bus";
		AddResult(sub == "bus", "String re assign test");

		sub = "Bus" + str_test;
		AddResult(sub == "BusHello, World!", "String re assign and add check");

		sub = "   sub   ";
		AddResult(sub.TrimStart() == "sub   ", "String trim start");
		AddResult(sub.TrimEnd() == "   sub", "String trim end");
		AddResult(sub.Trim() == "sub", "String trim");
		AddResult(sub.Trim().TrimEnd() == "sub", "String trim and trim end");
		

		sub = "   sub   ";
		sub.TrimStartInplace();
		AddResult(sub == "sub   ", "String trim start (inplace)");
		AddResult(sub.Length == 6, "String trim start length (inplace)");
		AddResult(sub.ByteLength == 6, "String trim start byte length (inplace)");

		sub = "   sub   ";
		sub.TrimEndInplace();
		AddResult(sub == "   sub", "String trim end (inplace)");
		AddResult(sub.Length == 6, "String trim end length (inplace)");
		AddResult(sub.ByteLength == 6, "String trim end byte length (inplace)");

		sub = "   sub   ";
		sub.TrimInplace();
		AddResult(sub == "sub", "String trim (inplace)");
		AddResult(sub.Length == 3, "String trim length (inplace)");
		AddResult(sub.ByteLength == 3, "String trim byte length (inplace)");

		string splittest = "01/12/2011";
		TArray<ch32> splitchars;
		splitchars.Add('/');

		TArray<TString*> SplitResult = splittest.Split( splitchars);
		AddResult(SplitResult.Count == 3, "String split result count");
		AddResult(*SplitResult.Item[0] == "01", "String split first element");
		AddResult(*SplitResult.Item[1] == "12", "String split second element");
		AddResult(*SplitResult.Item[2] == "2011", "String split third element");

		splitchars.Add('\\');

		splittest = "01\\12/2011";
		SplitResult = splittest.Split( splitchars );
		AddResult(SplitResult.Count == 3, "String split multi result count");
		AddResult(*SplitResult.Item[0] == "01", "String split multi first element");
		AddResult(*SplitResult.Item[1] == "12", "String split multi second element");
		AddResult(*SplitResult.Item[2] == "2011", "String split multi third element");

		splittest = "01//2011";
		SplitResult = splittest.Split( splitchars );
		AddResult(SplitResult.Count == 3, "String split multi result count, not remove empty");
		AddResult(*SplitResult.Item[0] == "01", "String split multi first element, not remove empty");
		AddResult(*SplitResult.Item[1] == TString::Empty, "String split multi second element, not remove empty");
		AddResult(*SplitResult.Item[2] == "2011", "String split multi third element, not remove empty");

		splittest = "01//2011";
		SplitResult = splittest.Split( splitchars, true );
		AddResult(SplitResult.Count == 2, "String split multi result count, remove empty");
		AddResult(*SplitResult.Item[0] == "01", "String split multi first element, remove empty");
		AddResult(*SplitResult.Item[1] == "2011", "String split multi second element, remove empty");

		TString fmttest = TString::Format("Test % % %", sfi(5), sfi(4), sfi(3));
		AddResult(fmttest == "Test 5 4 3", "String basic format test");

		fmttest = TString::Format("Test % % %", sfi(5,2,'0'),sfi(4,2,'0'),sfi(3,3,'0'));
		AddResult(fmttest == "Test 05 04 003", "String padded basic format test");

		fmttest = TString::Format("Test %", sfx(64));
		AddResult(fmttest == "Test 40", "String format hex test");

		// TODO: test unicode stuff here
	}

} TStringCheck;

#endif