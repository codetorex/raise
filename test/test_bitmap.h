#ifndef TEST_BITMAP_H
#define TEST_BITMAP_H

#include "ttest.h"
#include "tbitmap.h"
#include "tpath.h"

class TestTBitmap: public TTestCheck
{
public:
	TestTBitmap(): TTestCheck("TBitmap check") {};

	void TestFormat(const TString& path)
	{
		TBitmap bload(path); // first color of these files is TEST color
		TString border = MathColor::ChannelOrder(bload.Data, bload.BufferFormat->BytesPerItem);
		bool result = border == bload.BufferFormat->ShortName;

		TString fext = TPath::GetExtension(path).Substring(1);
		fext.ToUpperInplace();

		TStringBuilder sb;
		sb.Append( fext );
		sb.Append(" byte order and buffer order comparison");

		AddResult(result, sb.ToString());

		if (!result)
		{
			sb.Clear();
			sb.Append(fext);
			sb.Append(" is ");
			sb.Append(border);
			sb.Append(" , Buffer is ");
			sb.Append(bload.BufferFormat->ShortName);
			AddOutput(sb.ToString());
		}
	}

	void Test()
	{
		TestFormat("color-test.png");

		TestFormat("color-test.bmp");

		TestFormat("color-test.tga");

		TestFormat("color-test2.tga");
	}

} TBitmapCheck;

#endif