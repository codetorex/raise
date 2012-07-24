#include "stdafx.h"
#include "mmatrix.h"
#include "tstring.h"
#include "tconvert.h"



void MMatrix4x4::ToStringBuilder( TStringBuilder& sb )
{
	for (int y=0; y<4; y++)
	{
		for (int x=0;x<4;x++)
		{
			sb.Append(sff(m[y][x],4,10));
			sb.AppendChar(' ');
		}
		sb.AppendLine();
	}
}