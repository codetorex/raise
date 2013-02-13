#ifndef TLINKEDLIST_H
#define TLINKEDLIST_H

#include "raisetypes.h"
#include "tenumerator.h"
#include "texceptionlow.h"

/*template <class T>
class TSListNode // single linked list
{
public:
	T*	NextItem;
	TSListNode(): NextItem(0) {};
};

template <class T = TSListNode>
class TSList
{
	
};*/

/*template <class T>
class TList;

template <class T>
class TListNode
{
public:
	TListNode<T>* NextItem;
	TListNode<T>* PrevItem;
	T* Data;
	TListNode() : NextItem(0), PrevItem(0), Data(0) {};
};

template<class T>
class TList
{
public:
	typedef TListNode<T> Node;

	Node*	FirstItem;
	Node*	LastItem;
	int		ItemCount;

	TList(): FirstItem(0),LastItem(0),ItemCount(0) {};

	void Add (T* itm)
	{
		if (FirstItem == 0)
		{
			FirstItem = itm;
			LastItem = itm;
			itm->PrevItem = 0;
		}
		else
		{
			LastItem->NextItem = itm;
			itm->PrevItem = LastItem;
			LastItem = itm;
		}
		itm->NextItem = 0;
		ItemCount++;
	}

	void Remove(T* itm)
	{
		T *nx,*px;
		nx = itm->NextItem;
		px = itm->PrevItem;

		if (px)
		{
			px->NextItem = nx;
		}
		else
		{
			FirstItem = nx;
		}

		if (nx)
		{
			nx->PrevItem = px;
		}
		else
		{
			LastItem = px; // BUG FIX HMM!
		}

		itm->NextItem = 0;
		itm->PrevItem = 0;

		ItemCount--;
	}
};*/

/**
 * TODO: fix these shit. like use pointers instead of T and giving T as pointer because its pointless.
 */

template <class T>
class TList;

template <class T>
class TListNode
{
public:
	T			NextItem;
	T			PrevItem;
	TList<T>*	Parent;

	TListNode() : NextItem(0), PrevItem(0), Parent(0) {};
};


template <class T>
class TList
{
public:
	T		FirstItem;
	T		LastItem;
	int		ItemCount;

	TList(): FirstItem(0),LastItem(0),ItemCount(0) {};

	void Add (T itm)
	{
		if (FirstItem == 0)
		{
			FirstItem = itm;
			LastItem = itm;
			itm->PrevItem = 0;
		}
		else
		{
			LastItem->NextItem = itm;
			itm->PrevItem = LastItem;
			LastItem = itm;
		}
		itm->NextItem = 0;
		ItemCount++;
		itm->Parent = this;
	}

	void Remove(T itm)
	{
		T nx,px;
		nx = itm->NextItem;
		px = itm->PrevItem;

		if (px)
		{
			px->NextItem = nx;
		}
		else
		{
			FirstItem = nx;
		}

		if (nx)
		{
			nx->PrevItem = px;
		}
		else
		{
			LastItem = px; // BUG FIX HMM!
		}

		itm->NextItem = 0;
		itm->PrevItem = 0;
		itm->Parent = 0;

		ItemCount--;
	}

	/**
	 * Clears all sub items, warning POINTERS STILL REMAINS
	 */
	void ClearSubItems()
	{
		FirstItem = 0;
		LastItem = 0;
		ItemCount = 0;
	}

	/**
	 * DELETES items before clearance
	 */
	void ClearDelete()
	{
		LowLevelNotImplemented();
	}

	/**
	 * Merges lists together.
	 */
	void Merge( TList<T>* other )
	{
		LowLevelNotImplemented();
	}

	/**
	 * Deletes other list after merging.
	 */
	void MergeDelete(TList<T>* other)
	{
		LowLevelNotImplemented();
	}

	void InsertAfter(T baseItem , T itm)
	{
		if (baseItem->NextItem == 0)
		{
			Add(itm);
			return;
		}

		itm->Parent = this;
		itm->PrevItem = baseItem;
		itm->NextItem = baseItem->NextItem;
		baseItem->NextItem->PrevItem = itm;
		baseItem->NextItem = itm;
	}

	void InsertBefore(T baseItem, T itm)
	{
		if (baseItem->PrevItem == 0) // insert before first item
		{
			if (baseItem != FirstItem)
			{
				LowLevelException("Impposiblru error");
			}

			itm->Parent = this;
			itm->PrevItem = 0;
			itm->NextItem = baseItem;
			baseItem->PrevItem = itm;
			FirstItem = itm;
		}

		InsertAfter( baseItem->PrevItem, itm); // LOLZ
	}

	bool Contains( T itm );
};


template <class T>
class TLinkedListEnumerator: public TEnumerator<T>
{
public:
	TList<T>* mList;
	int CurrentIndex;

	TListNode<T> FakeFirst;

	TLinkedListEnumerator( TList<T>* pList ): mList(pList)
	{
		Reset();
	}

	void Reset()
	{
		CurrentIndex = 0;
		FakeFirst.NextItem = mList->FirstItem; // This guy saves us from checking in every moveNext call.
		this->Current = (T)&FakeFirst;
	}

	bool MoveNext()
	{
		this->Current = this->Current->NextItem;
		CurrentIndex++;
		return this->Current != 0;
	}
};

template <class T>
bool TList<T>::Contains( T itm )
{
	TLinkedListEnumerator<T> ll( this );

	while(ll.MoveNext())
	{
		if (ll.Current == itm)
		{
			return true;
		}
	}

	return false;
}

#endif