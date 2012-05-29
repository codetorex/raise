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

	void AlignBox(TBoxModel& srcBox, Alignment alignment)
	{
		int tempWidth = srcBox.Width;
		int tempHeight = srcBox.Height;
		srcBox.Inflate(srcBox.Margin); // we should count the margin of the item
		Align(srcBox,alignment);
		srcBox.Width = tempWidth;
		srcBox.Height = tempHeight;
	}

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

	/**
	 * Which is like in css width
	 */
	inline void SetContentWidth(int newWidth)
	{
		IPadding sum = Padding;
		sum += Border;

		ChangeWidth( newWidth + (sum.Left + sum.Right));
	}

	/**
	 * Which is like in css height
	 */
	inline void SetContentHeight(int newHeight)
	{
		IPadding sum = Padding;
		sum += Border;

		ChangeHeight( newHeight +( sum.Top + sum.Bottom));
	}

	/**
	 * Like in css width and height
	 */
	inline void SetContentSize( int newWidth, int newHeight)
	{
		SetContentWidth(newWidth);
		SetContentHeight(newHeight);
	}
};


#endif