#ifndef TEST_CONVERT_H
#define TEST_CONVERT_H

#include "ttest.h"
#include "tstring.h"

class TestTConvert: public UnitTest
{
public:
	TestTConvert(): UnitTest("TConvert check"){}

	void Execute()
	{
		Result( TConvert::ToUInt32Hex("0xffffffff") == 0xffffffff, "TConvert hex conversion test");
		Result( TConvert::ToUInt32Hex("3f") == 0x3f , "TConvert hex conversion test 2");
		Result( TConvert::ToUInt32Hex("2") == 0x2 , "TConvert hex conversion test 3");
		Result( TConvert::ToUInt32Hex("0x5") == 0x5 , "TConvert hex conversion test 4");
		Result( TConvert::ToUInt32Hex("0x3a5f1234") == 0x3a5f1234 , "TConvert hex conversion test 5");
	}

} TConvertCheck;


#endif