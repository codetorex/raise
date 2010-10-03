
#include "stdafx.h"
#include "tcolor.h"

void TColorHSL::ToHSL(const byte* rgb,byte* hsl)
{
	float var_R = ( (float)rgb[0] / 255.0f );                     //RGB from 0 to 255
	float var_G = ( (float)rgb[1] / 255.0f );
	float var_B = ( (float)rgb[2] / 255.0f );

	float var_Min = rmin( var_R, var_G, var_B );    //Min. value of RGB
	float var_Max = rmax( var_R, var_G, var_B );    //Max. value of RGB
	float del_Max = var_Max - var_Min;             //Delta RGB value

	float fH,fS;
	float fL = ( var_Max + var_Min ) / 2.0f;

	if ( del_Max == 0 )                     //This is a gray, no chroma...
	{
		hsl[0] = 0;                                //HSL results from 0 to 1
		hsl[1] = 0;
		hsl[2] = (byte)(fL * 255.0f);
	}
	else                                    //Chromatic data...
	{
		if ( fL < 0.5 )
		{
			fS = del_Max / ( var_Max + var_Min );
		}
		else
		{
			fS = del_Max / ( 2 - var_Max - var_Min );
		}

		float del_R = ( ( ( var_Max - var_R ) / 6.0f ) + ( del_Max / 2.0f ) ) / del_Max;
		float del_G = ( ( ( var_Max - var_G ) / 6.0f ) + ( del_Max / 2.0f ) ) / del_Max;
		float del_B = ( ( ( var_Max - var_B ) / 6.0f ) + ( del_Max / 2.0f ) ) / del_Max;

		if      ( var_R == var_Max ) fH = del_B - del_G;
		else if ( var_G == var_Max ) fH = ( 1.0f / 3.0f ) + del_R - del_B;
		else if ( var_B == var_Max ) fH = ( 2.0f / 3.0f ) + del_G - del_R;

		if ( fH < 0.0f ) fH += 1.0f;
		if ( fH > 1.0f ) fH -= 1.0f;

		hsl[0] = (byte)(fH * 255.0f);
		hsl[1] = (byte)(fS * 255.0f);
		hsl[2] = (byte)(fL * 255.0f);
	}
}

float Hue_2_RGB( float v1,float v2,float vH )
{
	if ( vH < 0 ) vH += 1;
	if ( vH > 1 ) vH -= 1;
	if ( ( 6.0f * vH ) < 1 ) return ( v1 + ( v2 - v1 ) * 6.0f * vH );
	if ( ( 2.0f * vH ) < 1 ) return ( v2 );
	if ( ( 3.0f * vH ) < 2 ) return ( v1 + ( v2 - v1 ) * ( ( 2.0f / 3.0f ) - vH ) * 6.0f );
	return ( v1 );
}

void TColorHSL::ToRGB( const byte* hsl,byte* rgb )
{
	float fH,fS,fL;
	if ( hsl[1] == 0 )
	{
		rgb[0] = hsl[2];
		rgb[1] = rgb[1];
		rgb[2] = rgb[1];
	}
	else
	{
		float var_1,var_2;
		fH = (float)hsl[0] / 255.0f;
		fS = (float)hsl[1] / 255.0f;
		fL = (float)hsl[2] / 255.0f;

		if ( fL < 0.5 ) 
		{
			var_2 = fL * ( 1 + fS );
		}
		else
		{
			var_2 = ( fL + fS ) - ( fS * fL );
		}

		var_1 = 2 * fL - var_2;

		rgb[0] = (byte)(255.0f * Hue_2_RGB( var_1, var_2, fH + ( 1.0f / 3.0f ) ));
		rgb[1] = (byte)(255.0f * Hue_2_RGB( var_1, var_2, fH ));
		rgb[2] = (byte)(255.0f * Hue_2_RGB( var_1, var_2, fH - ( 1.0f / 3.0f ) ));
	}
}