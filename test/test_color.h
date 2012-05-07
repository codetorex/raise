#ifndef TEST_COLOR_H
#define TEST_COLOR_H

#include "ttest.h"
#include "tcolor.h"
#include "mmathcolor.h"


class TestTColor: public TTestCheck
{
public:
	TestTColor(): TTestCheck("TColor check") {};



	void Test()
	{
		// TODO: do more testing

		TColor32 clr(64,128,178,255);

		TColor32 testcolor ( TColors::TestColor );

		TStringBuilder sb;
		sb.Append("Test color hex:");
		sb.Append(sfx(clr.color));
		AddOutput(sb.ToString());
		
		sb.Clear();
		sb.Append("Input:");
		sb.Append(" R: ");
		sb.AppendPadded(64,3,' ');
		sb.Append(" G: ");
		sb.AppendPadded(128,3,' ');
		sb.Append(" B: ");
		sb.AppendPadded(178,3,' ');
		sb.Append(" A: ");
		sb.AppendPadded(255,3,' ');
		AddOutput(sb.ToString());

		sb.Clear();
		sb.Append("Color:");
		sb.Append(" R: ");
		sb.AppendPadded(clr.r,3,' ');
		sb.Append(" G: ");
		sb.AppendPadded(clr.g,3,' ');
		sb.Append(" B: ");
		sb.AppendPadded(clr.b,3,' ');
		sb.Append(" A: ");
		sb.AppendPadded(clr.a,3,' ');
		AddOutput(sb.ToString());

		sb.Clear();
		sb.Append("clr[0] = ");
		sb.AppendPadded(clr.bclr[0],3,' ');
		sb.Append("clr[1] = ");
		sb.AppendPadded(clr.bclr[1],3,' ');
		sb.Append("clr[2] = ");
		sb.AppendPadded(clr.bclr[2],3,' ');
		sb.Append("clr[3] = ");
		sb.AppendPadded(clr.bclr[3],3,' ');
		AddOutput(sb.ToString());
			
		sb.Clear();
		sb.Append("Byte Order: ");
		sb.Append(MathColor::ChannelOrder(clr.bclr,4));
		AddOutput(sb.ToString());


		sb.Clear();
		TBitmap pngload("color-test.png");
		sb.Append("PNG Byte Order: ");
		sb.Append(MathColor::ChannelOrder(pngload.Data,4));
		AddOutput(sb.ToString());

		sb.Clear();
		TBitmap bmpload("color-test.bmp");
		sb.Append("BMP Byte Order: ");
		sb.Append(MathColor::ChannelOrder(bmpload.Data,3));
		AddOutput(sb.ToString());

		AddResult( clr.r == 64 && clr.g == 128 && clr.b == 178 && clr.a == 255, "Color32 r,g,b,a constructor" );
		
		AddResult( clr == testcolor, "Color32 r,g,b,a constructor == TColors::TestColor");

		clr = TColors::Red;
		AddResult( clr.r == 255 && clr.g == 0 && clr.b == 0 && clr.a == 255, "Color32 = ui32 operator for red" );

		sb.Clear();
		sb.Append("Red Color:");
		sb.Append(" R: ");
		sb.AppendPadded(clr.r,3,' ');
		sb.Append(" G: ");
		sb.AppendPadded(clr.g,3,' ');
		sb.Append(" B: ");
		sb.AppendPadded(clr.b,3,' ');
		sb.Append(" A: ");
		sb.AppendPadded(clr.a,3,' ');
		AddOutput(sb.ToString());

		clr = TColors::Aqua;
		AddResult( clr.r == 0 && clr.g == 255 && clr.b == 255 && clr.a == 255, "Color32 = ui32 operator for aqua" );

		clr = TColors::TestColor;
		sb.Clear();
		sb.Append("Byte Order: ");
		sb.Append(MathColor::ChannelOrder(clr.bclr,4));
		AddOutput(sb.ToString());
	}
} TColorCheck;


#endif