#ifndef TEST_FALCONRENDERER_H
#define TEST_FALCONRENDERER_H

#include "ttest.h"
#include "wfalconrenderer.h"


class TestFalconRenderer: public UnitTest
{
public:
	TestFalconRenderer(): UnitTest("WFalconRenderer check") {};


	bool CheckFragment(WFalconFragment* frg, int typ, const String& val)
	{
		if (frg->Type == typ && frg->Fragment == val)
		{
			return true;
		}
		return false;
	}

	void Execute()
	{
		Should("Parse @@ as @ in text");
		{
			WFalconRenderer renderer;
			renderer.SetSource("<html>@@</html>");
			renderer.ParseSource();

			bool result = CheckFragment(renderer.Fragments[0], FFT_TEXT, "<html>@</html>" );
			Result(result);
		}

		Should("Parse code comes after @ as code until it reaches html part");
		{
			WFalconRenderer renderer;
			renderer.SetSource("<html><img src=\"@Url.Content(\"~/Contents/Images/img.jpg\")\"></html>");
			renderer.ParseSource();
			
			bool result0 = CheckFragment(renderer.Fragments[0], FFT_TEXT, "<html><img src=\"" );
			bool result1 = CheckFragment(renderer.Fragments[1], FFT_CODE, "Url.Content(\"~/Contents/Images/img.jpg\")" );
			bool result2 = CheckFragment(renderer.Fragments[2], FFT_TEXT, "\"></html>" );
			Result(result0 && result1 && result2);
		}
	}

} WFalconRendererCheck;


#endif