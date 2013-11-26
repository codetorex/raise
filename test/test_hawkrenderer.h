#ifndef TEST_HAWKRENDERER_H
#define TEST_HAWKRENDERER_H

#include "ttest.h"
#include "whawkrenderer.h"
#include "tstringreader.h"


class TestHawkRenderer: public UnitTest
{
public:
	TestHawkRenderer(): UnitTest("WHawkRenderer check") {};


	bool CheckFragment(WHawkFragment* frg, int typ, const String& val)
	{
		if (frg->Type == typ && frg->Fragment == val)
		{
			return true;
		}
		return false;
	}

	void Execute()
	{
		Should("Parse text and code in a mixed string");
		{
			WHawkRenderer renderer;
			TStringReader sr("Test text <$ Test Code $> End Text");
			renderer.ParseSource(sr); // creates fragments from given source file

			Should("Have 3 fragments in given test string");
			if (renderer.Fragments.Count == 3)
			{
				Result(true);

				Should("Parse as text fragment until it reaches <$");
				bool result = CheckFragment(renderer.Fragments[0], WHawkFragment::HFTYP_TEXT, "Test text " );
				Result(result);

				Should("Parse between <$ $> as code");
				result = CheckFragment(renderer.Fragments[1], WHawkFragment::HFTYP_CODE, " Test Code " );
				Result(result);

				Should("Parse remaining part as text");
				result = CheckFragment(renderer.Fragments[2], WHawkFragment::HFTYP_TEXT, " End Text" );
				Result(result);
			}
			else
			{
				Result(false);
			}
		}

		Should("Parse between <$ $> as code");
		{
			WHawkRenderer renderer;
			TStringReader sr("<$ Test Code $>");
			renderer.ParseSource(sr); // creates fragments from given source file

			bool result = CheckFragment(renderer.Fragments[0], WHawkFragment::HFTYP_CODE, " Test Code ");
			Result(result);

			Should("Only have 1 code fragment"); // TODO: implement AlsoShould function.
			Result(renderer.Fragments.Count == 1);
		}

		Should("Parse as text if there is no code");
		{
			WHawkRenderer renderer;
			TStringReader sr("Test Text");
			renderer.ParseSource(sr); // creates fragments from given source file

			bool result = CheckFragment(renderer.Fragments[0], WHawkFragment::HFTYP_TEXT, "Test Text");
			Result(result);

			Should("Only have 1 text fragment"); // TODO: implement AlsoShould function.
			Result(renderer.Fragments.Count == 1);
		}

		Should("Parse remaining as code if no end of code found");
		{
			WHawkRenderer renderer;
			TStringReader sr("Test Text <$ Test Code");
			renderer.ParseSource(sr); // creates fragments from given source file

			bool result = CheckFragment(renderer.Fragments[0], WHawkFragment::HFTYP_TEXT, "Test Text ");
			Result(result);

			result = CheckFragment(renderer.Fragments[1], WHawkFragment::HFTYP_CODE, " Test Code");
			Result(result);

			Should("Only have 2 fragments"); // TODO: implement AlsoShould function.
			Result(renderer.Fragments.Count == 2);
		}
	}

} WHawkRendererCheck;


#endif