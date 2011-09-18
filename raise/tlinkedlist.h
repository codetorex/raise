#ifndef TLINKEDLIST_H
#define TLINKEDLIST_H

#include "raisetypes.h"
#include "tenumerator.h"

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

	}

	void Reset()
	{
		CurrentIndex = 0;
		FakeFirst.NextItem = mList->FirstItem; // This guy saves us from checking in every moveNext call.
		Current = (T)&FakeFirst;
	}

	bool MoveNext()
	{
		Current = Current->NextItem;
		CurrentIndex++;
		return Current != 0;
	}
};



#endif