#ifndef THASHMAP_H
#define THASHMAP_H

#include "raisetypes.h"
#include "tstring.h"
#include "tkeyvalue.h"
#include "thashcodeprovider.h"
#include "tenumerator.h"



template <class T>
class RDLL THashKeyValue: public TKeyValue<String,T>
{
public:
	ui32 Hash;

	THashKeyValue(ui32 h,const String& k, T v)
	{
		this->Hash = h;
		this->Key = k;
		this->Value = v;
	}
};


template <class T>
class RDLL TMapNode
{
public:
	TMapNode()
	{
		valueCount = 0;
	}

	int valueCount;
	bool leaf; // true if its a leaf, false if branch

	inline virtual THashKeyValue<T>* Get(ui32 h,const String& key) = 0;
	inline virtual void Add(ui32 h,THashKeyValue<T>* value) = 0;
};


template <class T>
class RDLL TMapLeaf: public TMapNode<T>
{
public:
	TMapLeaf<T>()
	{
		this->leaf = true;
	}

	THashKeyValue<T>* values[4];

	inline THashKeyValue<T>* Get(ui32 h,const String& key)
	{
		int i=this->valueCount;
		while(i--)
		{
			if ( values[i]->Hash == h)
			{
				if (values[i]->Key == key) // maybe this step is unnecessary if you trust your hash
				{
					return values[i];
				}
			}
		}
		return NULL;
	}


	inline void Add(ui32 h,THashKeyValue<T>* value)
	{
		values[this->valueCount++] = value;
	}
};

template <class T>
class RDLL TMapBranch: public TMapNode<T>
{
private:
	void Initialize(int _branchLevel)
	{
		int i=32;
		while(i--)
		{
			Map[i] = 0;
		}
		this->leaf = false;
		branchLevel = _branchLevel;
		branchShift = branchLevel * 8;
	}

public:
	TMapBranch()
	{
		Initialize(0);
	}

	TMapBranch(int lvl)
	{
		Initialize(lvl);
	}

	int branchLevel;
	int branchShift;
	TMapNode<T>* Map[32];

	/**
	* Converts leaf to branch.
	*/
	void Grow(int index)
	{
		TMapLeaf<T>* oldLeaf = (TMapLeaf<T>*) Map[index];
		TMapBranch<T>* brnc = new TMapBranch<T>(branchLevel + 1); // one more than my level
		Map[index] = brnc;
		int i = oldLeaf->valueCount;

		THashKeyValue<T>* kvp;
		while (i--)
		{
			kvp = oldLeaf->values[i];
			brnc->Add(kvp->Hash,kvp);
		}

		delete oldLeaf;
	}

	inline THashKeyValue<T>* Get(ui32 h,const String& key)
	{
		int i = (h >> branchShift) % 32;
		//h >>= 8;
		if (Map[i] != 0)
		{
			return Map[i]->Get(h,key);
		}
		return NULL;
	}

	inline void Add(ui32 h,THashKeyValue<T>* value)
	{
		int i = (h >> branchShift) % 32;
		if (Map[i] != 0)
		{
			if ( Map[i]->leaf && Map[i]->valueCount == 4 )
			{
				Grow(i);
			}
		}
		else
		{
			Map[i] = new TMapLeaf<T>();
		}
		Map[i]->Add(h,value);
	}
};

/**
* An hash map implementation. That can hash 4194304 elements.
* Type is used for VALUE.
*/
template <class T>
class RDLL THashMap
{
public:
	int Count;
	TMapBranch<T> Root;

	THashMap()
	{
		Count = 0;
	}

	inline bool ContainsKey(const String& key)
	{
		ui32 h = TBasicHashCodeProvider::Instance.GetHashCode(key); // TString::GetHash(key);
		THashKeyValue<T>* r = Root.Get(h,key);
		if (r == NULL)
		{
			return false;
		}
		return true;
	}

	inline THashKeyValue<T>* Get(const String& key)
	{
		ui32 h = TBasicHashCodeProvider::Instance.GetHashCode(key);
		THashKeyValue<T>* r = Root.Get(h,key);
		return r;
	}

	inline T GetValue(const String& key)
	{
		THashKeyValue<T>* tkp = Get(key);
		if (tkp == NULL)
		{
			throw Exception("Key not found");
		}
		return tkp->Value;
	}

	inline T GetValueOrNull( const String& key)
	{
		THashKeyValue<T>* tkp = Get(key);
		if (tkp == NULL)
		{
			return 0;
		}
		return tkp->Value;
	}

	inline void Add(const String& key,T value)
	{
		ui32 h = TBasicHashCodeProvider::Instance.GetHashCode(key); // TString::GetHash(key);
		THashKeyValue<T>* kvp = new THashKeyValue<T>(h,key,value);
		Root.Add(h,kvp);
		Count++;
	}
};

template<class T>
class THashMapEnumerator: public TEnumerator< THashKeyValue<T>* >
{
private:
	THashMap<T>* hmap;
	TMapBranch<T>* rootBranch;

	int LastLeafItem;
	int Levels[4]; // will hold branch levels
	int CurLevel;

	bool NoItemLeft;

	THashKeyValue<T>* LoopLeaf(TMapLeaf<T>* pLeaf)
	{
		for (;LastLeafItem < pLeaf->valueCount;)
		{
			return pLeaf->values[LastLeafItem++];
		}

		LastLeafItem = 0;
		return 0;
	}

	bool LoopLevel(int level, TMapBranch<T>* branch)
	{
		int& levelCounter = Levels[level];
		for (; levelCounter < 32; levelCounter++)
		{
			CurLevel = level;
			TMapNode<T>* curNode = branch->Map[ levelCounter ];
			if (!curNode) continue;
			if (curNode->leaf)
			{
				this->Current = LoopLeaf( (TMapLeaf<T>*)curNode );
				if (this->Current) return true;
			}
			else
			{
				bool result = LoopLevel( level + 1, (TMapBranch<T>*)curNode);
				if (result) return true;
			}
		}

		levelCounter = 0;

		if (level == 0)
		{
			NoItemLeft = true;
		}

		return false;
	}

public:


	THashMapEnumerator( THashMap<T>* _hmap )
	{
		hmap = _hmap;
		rootBranch = &hmap->Root;
		Reset();
	}

	void Reset()
	{
		Levels[0] = Levels[1] = Levels[2] = Levels[3] = 0;
		LastLeafItem= 0;
		NoItemLeft=false;
	}

	bool MoveNext()
	{
		if (NoItemLeft)
		{
			return false;
		}
		return LoopLevel(0,rootBranch);
	}
};

// TODO: implement a preallocated MEMORY CACHE MANAGER for easy allocation.

#endif