#ifndef TEST_MARKDOWN_H
#define TEST_MARKDOWN_H

#include "ttest.h"
#include "traisemarkdown.h"

class TestTRaiseMarkdown: public UnitTest
{
public:

	TestTRaiseMarkdown(): UnitTest("TRaiseMarkdown check") 
	{

	}

	void HeadingTest()
	{
		Should("Convert lines appear entirely of = to headings");
		{
			TString testStr = "Test Heading\n==";
			TRaiseMarkdown markdown;
			markdown.Parse(testStr);
			Result(markdown.Elements[0].Type == MET_HEADING1 && markdown.Elements[0].Text == "Test Heading");
		}


		Should("Convert lines beginning with # to heading 1");
		{
			TString testStr = "# Test Heading";
			TRaiseMarkdown markdown;
			markdown.Parse(testStr);
			Result(markdown.Elements[0].Type == MET_HEADING1 && markdown.Elements[0].Text == "Test Heading");
		}


		Should("Convert lines beginning with ## to heading 2");
		{
			TString testStr = "## Test Heading";
			TRaiseMarkdown markdown;
			markdown.Parse(testStr);
			Result(markdown.Elements[0].Type == MET_HEADING2 && markdown.Elements[0].Text == "Test Heading");
		}


		Should("Convert lines beginning with ### to heading 3");
		{
			TString testStr = "### Test Heading";
			TRaiseMarkdown markdown;
			markdown.Parse(testStr);
			Result(markdown.Elements[0].Type == MET_HEADING3 && markdown.Elements[0].Text == "Test Heading");
		}


		Should("Convert lines beginning with #### to heading 4");
		{
			TString testStr = "#### Test Heading";
			TRaiseMarkdown markdown;
			markdown.Parse(testStr);
			Result(markdown.Elements[0].Type == MET_HEADING4 && markdown.Elements[0].Text == "Test Heading");
		}
	}

	void Execute()
	{
		HeadingTest();


	}

} TRaiseMarkdownCheck;

#endif