#ifndef THASHMAP_H
#define THASHMAP_H

#include "raisetypes.h"
#include "tstring.h"


template <class T>
class RDLL TKeyValuePair
{
public:

	TKeyValuePair(dword h,str8& k, T v)
	{
		hsh = h;
		key = k;
		value = v;
	}

	dword hsh;
	str8 key; // so key will be copied
	T value;
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

	inline virtual TKeyValuePair<T>* Get(dword h,const str8& key) = 0;
	inline virtual void Add(dword h,TKeyValuePair<T>* value) = 0;
};


template <class T>
class RDLL TMapLeaf: public TMapNode<T>
{
public:
	TMapLeaf()
	{
		leaf = true;
	}

	TKeyValuePair<T>* values[4];

	inline TKeyValuePair<T>* Get(dword h,const str8& key)
	{
		int i=valueCount;
		while(i--)
		{
			if ( values[i]->hsh == h)
			{
				if (values[i]->key == key) // maybe this step is unnecessary if you trust your hash
				{
					return values[i];
				}
			}
		}
		return NULL;
	}


	inline void Add(dword h,TKeyValuePair<T>* value)
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

		TKeyValuePair<T>* kvp;
		while (i--)
		{
			kvp = oldLeaf->values[i];
			brnc->Add(kvp->hsh,kvp);
		}

		delete oldLeaf;
	}

	inline TKeyValuePair<T>* Get(dword h,const str8& key)
	{
		int i = MOD32(h >> branchShift);
		//h >>= 8;
		if (Map[i] != 0)
		{
			return Map[i]->Get(h,key);
		}
		return NULL;
	}

	inline void Add(dword h,TKeyValuePair<T>* value)
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
	int totalValues;
	TMapBranch<T> Root;

	inline bool ContainsKey(const str8& key)
	{
		dword h = str8::GetHash(key);
		TKeyValuePair<T>* r = Root.Get(h,key);
		if (r == NULL)
		{
			return false;
		}
		return true;
	}

	inline TKeyValuePair<T>* Get(const str8& key)
	{
		dword h = str8::GetHash(key);
		TKeyValuePair<T>* r = Root.Get(h,key);
		return r;
	}

	inline T GetValue(const str8& key)
	{
		TKeyValuePair<T>* tkp = Get(key);
		if (tkp == NULL)
		{
			throw Exception("Key not found");
		}
		return tkp->value;
	}

	inline void Add(str8& key,T value)
	{
		dword h = str8::GetHash(key);
		TKeyValuePair<T>* kvp = new TKeyValuePair<T>(h,key,value);
		Root.Add(h,kvp);
	}
};

// TODO: implement a preallocated MEMORY CACHE MANAGER for easy allocation.

#endif