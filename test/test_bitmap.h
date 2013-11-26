#ifndef TEST_BITMAP_H
#define TEST_BITMAP_H

#include "ttest.h"
#include "tbitmap.h"
#include "tpath.h"

class TestTBitmap: public UnitTest
{
public:
	TestTBitmap(): UnitTest("TBitmap check") {};

	void TestFormat(const String& path)
	{
		TBitmap bload(path); // first color of these files is TEST color
		String border = MathColor::ChannelOrder(bload.Data, bload.BufferFormat->BytesPerItem);
		bool result = border == bload.BufferFormat->ShortName;

		String fext = TPath::GetExtension(path).Substring(1);
		fext.ToUpperInplace();

		TStringBuilderStack<512> sb;
		sb.Append( fext );
		sb.Append(" byte order and buffer order comparison");

		Result(result, sb.ToString());

		if (!result)
		{
			sb.Clear();
			sb.Append(fext);
			sb.Append(" is ");
			sb.Append(border);
			sb.Append(" , Buffer is ");
			sb.Append(bload.BufferFormat->ShortName);
			Output(sb.ToString());
		}
	}

	void Execute()
	{
		TestFormat("color-test.png");

		TestFormat("color-test.bmp");

		TestFormat("color-test.tga"); // 24 bit written tga

		//TestFormat("color-test2.tga"); 32 bit written one
	}

} TBitmapCheck;

#endif