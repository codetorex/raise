#ifndef TBOXMODEL_H
#define TBOXMODEL_H

#include "mregion2.h"

/**
 * Region defines total width, total height
 */
class TBoxModel: public IRectangle
{
public:
	IPadding Margin; // margin around
	IPadding Padding; // client rect padding
	IPadding Border; // skin border

	/// Relative content rectangle
	IRectangle Content;

	/// Where actual draw and event handling occurs
	IRegion DrawRegion;

	/**
	 * Call this after Margin, Padding , Border Width or Height changes.
	 */
	void UpdateContent()
	{
		IPadding sum = Margin;
		sum += Padding;
		sum += Border;

		Content.SetFromPadding(Width,Height,sum);
	}

	/**
	 * Updates draw region by parents draw region
	 * You should call this whenever greatest parent's draw region changes.
	 */
	void UpdateDrawRegion(TBoxModel& parent)
	{
		int nX = parent.DrawRegion.X() + parent.Content.X + X;
		int nY = parent.DrawRegion.Y() + parent.Content.Y + Y;
		DrawRegion.SetRegion(nX,nY,Width,Height);
	}
};


#endif