#ifndef TEST_HAWKRENDERER_H
#define TEST_HAWKRENDERER_H

#include "ttest.h"
#include "whawkrenderer.h"
#include "tstringreader.h"


class TestHawkRenderer: public TTestCheck
{
public:
	TestHawkRenderer(): TTestCheck("WHawkRenderer check") {};


	bool CheckFragment(WHawkFragment* frg, int typ, const TString& val)
	{
		if (frg->Type == typ && frg->Fragment == val)
		{
			return true;
		}
		return false;
	}

	void Test()
	{
		Should("Parse text and code in a mixed string");
		{
			WHawkRenderer renderer;
			TStringReader sr("Test text <$ Test Code $> End Text");
			renderer.ParseSource(sr); // creates fragments from given source file

			Should("Have 3 fragments in given test string");
			if (renderer.Fragments.Count == 3)
			{
				AddResult(true);

				Should("Parse as text fragment until it reaches <$");
				bool result = CheckFragment(renderer.Fragments[0], WHawkFragment::HFTYP_TEXT, "Test text " );
				AddResult(result);

				Should("Parse between <$ $> as code");
				result = CheckFragment(renderer.Fragments[1], WHawkFragment::HFTYP_CODE, " Test Code " );
				AddResult(result);

				Should("Parse remaining part as text");
				result = CheckFragment(renderer.Fragments[2], WHawkFragment::HFTYP_TEXT, " End Text" );
				AddResult(result);
			}
			else
			{
				AddResult(false);
			}
		}

		Should("Parse between <$ $> as code");
		{
			WHawkRenderer renderer;
			TStringReader sr("<$ Test Code $>");
			renderer.ParseSource(sr); // creates fragments from given source file

			bool result = CheckFragment(renderer.Fragments[0], WHawkFragment::HFTYP_CODE, " Test Code ");
			AddResult(result);

			Should("Only have 1 code fragment"); // TODO: implement AlsoShould function.
			AddResult(renderer.Fragments.Count == 1);
		}

		Should("Parse as text if there is no code");
		{
			WHawkRenderer renderer;
			TStringReader sr("Test Text");
			renderer.ParseSource(sr); // creates fragments from given source file

			bool result = CheckFragment(renderer.Fragments[0], WHawkFragment::HFTYP_TEXT, "Test Text");
			AddResult(result);

			Should("Only have 1 text fragment"); // TODO: implement AlsoShould function.
			AddResult(renderer.Fragments.Count == 1);
		}

		Should("Parse remaining as code if no end of code found");
		{
			WHawkRenderer renderer;
			TStringReader sr("Test Text <$ Test Code");
			renderer.ParseSource(sr); // creates fragments from given source file

			bool result = CheckFragment(renderer.Fragments[0], WHawkFragment::HFTYP_TEXT, "Test Text ");
			AddResult(result);

			result = CheckFragment(renderer.Fragments[1], WHawkFragment::HFTYP_CODE, " Test Code");
			AddResult(result);

			Should("Only have 2 fragments"); // TODO: implement AlsoShould function.
			AddResult(renderer.Fragments.Count == 2);
		}
	}

} WHawkRendererCheck;


#endif