#ifndef TEST_CHARACTERENUMERATOR_H
#define TEST_CHARACTERENUMERATOR_H

#include "ttest.h"

class TestTCharacterEnumerator: public UnitTest
{
public:
	TestTCharacterEnumerator(): UnitTest("TCharacterEnumerator check") {};

	void Execute()
	{
		TString realizer = "realizer";

		TCharacterEnumerator ce(realizer);
		int curChar = 0;

		while(ce.MoveNext())
		{
			switch(curChar)
			{
			case 0:
				Result(ce.Current == 'r', "CharacterEnumerator realizer char 0");
				Result(ce.CharIndex == 0, "CharacterEnumerator realizer char index 0");
				break;

			case 1:
				Result(ce.Current == 'e', "CharacterEnumerator realizer char 1");
				Result(ce.CharIndex == 1, "CharacterEnumerator realizer char index 1");
				break;

			case 2:
				Result(ce.Current == 'a', "CharacterEnumerator realizer char 2");
				Result(ce.CharIndex == 2, "CharacterEnumerator realizer char index 2");
				break;

			case 3:
				Result(ce.Current == 'l', "CharacterEnumerator realizer char 3");
				Result(ce.CharIndex == 3, "CharacterEnumerator realizer char index 3");
				break;

			case 4:
				Result(ce.Current == 'i', "CharacterEnumerator realizer char 4");
				Result(ce.CharIndex == 4, "CharacterEnumerator realizer char index 4");
				break;

			case 5:
				Result(ce.Current == 'z', "CharacterEnumerator realizer char 5");
				Result(ce.CharIndex == 5, "CharacterEnumerator realizer char index 5");
				break;

			case 6:
				Result(ce.Current == 'e', "CharacterEnumerator realizer char 6");
				Result(ce.CharIndex == 6, "CharacterEnumerator realizer char index 6");
				break;

			case 7:
				Result(ce.Current == 'r', "CharacterEnumerator realizer char 7");
				Result(ce.CharIndex == 7, "CharacterEnumerator realizer char index 7");
				break;

			case 8:
				Result(false, "CharacterEnumerator passes length of string");
				break;
			}

			curChar ++;
		}

	}
} TCharacterEnumeratorCheck;



#endif