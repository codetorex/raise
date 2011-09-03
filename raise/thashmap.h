#ifndef THASHMAP_H
#define THASHMAP_H

#include "raisetypes.h"
#include "tstring.h"
#include "tkeyvalue.h"
#include "thashcodeprovider.h"



template <class T>
class RDLL THashKeyValue: public TKeyValue<TString,T>
{
public:
	dword Hash;

	THashKeyValue(dword h,TString& k, T v)
	{
		Hash = h;
		Key = k;
		Value = v;
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

	inline virtual THashKeyValue<T>* Get(dword h,const TString& key) = 0;
	inline virtual void Add(dword h,THashKeyValue<T>* value) = 0;
};


template <class T>
class RDLL TMapLeaf: public TMapNode<T>
{
public:
	TMapLeaf()
	{
		leaf = true;
	}

	THashKeyValue<T>* values[4];

	inline THashKeyValue<T>* Get(dword h,const TString& key)
	{
		int i=valueCount;
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


	inline void Add(dword h,THashKeyValue<T>* value)
	{
		values[valueCount++] = value;
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
		leaf = false;
		branchLevel = _branchLevel;
		branchShift = MUL8(branchLevel);
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

	inline THashKeyValue<T>* Get(dword h,const TString& key)
	{
		int i = MOD32(h >> branchShift);
		//h >>= 8;
		if (Map[i] != 0)
		{
			return Map[i]->Get(h,key);
		}
		return NULL;
	}

	inline void Add(dword h,THashKeyValue<T>* value)
	{
		int i = MOD32(h >> branchShift);
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

	inline bool ContainsKey(const TString& key)
	{
		dword h = TBasicHashCodeProvider::Instance.GetHashCode(key); // TString::GetHash(key);
		THashKeyValue<T>* r = Root.Get(h,key);
		if (r == NULL)
		{
			return false;
		}
		return true;
	}

	inline THashKeyValue<T>* Get(const TString& key)
	{
		dword h = TBasicHashCodeProvider::Instance.GetHashCode(key);
		THashKeyValue<T>* r = Root.Get(h,key);
		return r;
	}

	inline T GetValue(const TString& key)
	{
		THashKeyValue<T>* tkp = Get(key);
		if (tkp == NULL)
		{
			throw Exception("Key not found");
		}
		return tkp->Value;
	}

	inline void Add(TString& key,T value)
	{
		dword h = TBasicHashCodeProvider::Instance.GetHashCode(key); // TString::GetHash(key);
		THashKeyValue<T>* kvp = new THashKeyValue<T>(h,key,value);
		Root.Add(h,kvp);
		Count++;
	}
};

// TODO: implement a preallocated MEMORY CACHE MANAGER for easy allocation.

#endif