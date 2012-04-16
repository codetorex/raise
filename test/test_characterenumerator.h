#ifndef TEST_CHARACTERENUMERATOR_H
#define TEST_CHARACTERENUMERATOR_H

#include "ttest.h"

class TestTCharacterEnumerator: public TTestCheck
{
public:
	TestTCharacterEnumerator(): TTestCheck("TCharacterEnumerator check") {};

	void Test()
	{
		TString realizer = "realizer";

		TCharacterEnumerator ce(realizer);
		int curChar = 0;

		while(ce.MoveNext())
		{
			switch(curChar)
			{
			case 0:
				AddResult(ce.Current == 'r', "CharacterEnumerator realizer char 0");
				AddResult(ce.CharIndex == 0, "CharacterEnumerator realizer char index 0");
				break;

			case 1:
				AddResult(ce.Current == 'e', "CharacterEnumerator realizer char 1");
				AddResult(ce.CharIndex == 1, "CharacterEnumerator realizer char index 1");
				break;

			case 2:
				AddResult(ce.Current == 'a', "CharacterEnumerator realizer char 2");
				AddResult(ce.CharIndex == 2, "CharacterEnumerator realizer char index 2");
				break;

			case 3:
				AddResult(ce.Current == 'l', "CharacterEnumerator realizer char 3");
				AddResult(ce.CharIndex == 3, "CharacterEnumerator realizer char index 3");
				break;

			case 4:
				AddResult(ce.Current == 'i', "CharacterEnumerator realizer char 4");
				AddResult(ce.CharIndex == 4, "CharacterEnumerator realizer char index 4");
				break;

			case 5:
				AddResult(ce.Current == 'z', "CharacterEnumerator realizer char 5");
				AddResult(ce.CharIndex == 5, "CharacterEnumerator realizer char index 5");
				break;

			case 6:
				AddResult(ce.Current == 'e', "CharacterEnumerator realizer char 6");
				AddResult(ce.CharIndex == 6, "CharacterEnumerator realizer char index 6");
				break;

			case 7:
				AddResult(ce.Current == 'r', "CharacterEnumerator realizer char 7");
				AddResult(ce.CharIndex == 7, "CharacterEnumerator realizer char index 7");
				break;

			case 8:
				AddResult(false, "CharacterEnumerator passes length of string");
				break;
			}

			curChar ++;
		}

	}
} TCharacterEnumeratorCheck;



#endif