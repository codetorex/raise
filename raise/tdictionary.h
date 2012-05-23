#ifndef TDICTIONARY_H
#define TDICTIONARY_H

#include "tkeyvalue.h"

template <class T>
class RDLL TDictonaryPage
{
public:
	TArray< TKeyValue<TString,T>* > Values;
};

template <class T>
class RDLL TDictionary
{
public:
	int totalValues;
	TDictonaryPage<T> Map[32];

	TDictionary()
	{
		totalValues = 0;
	}

	/**
	 * @brief Gets a value from dictionary.
	 * @param key Key for lookup in dictionary.
	 * @return The value of key.
	 */
	T Get(const TString& key)
	{
		int page = MOD32(key.Data[0]);
		for (int i=0;i<Map[page].values.Count;i++)
		{
			TKeyValue<TString,T>* curKV = Map[page].Values.Item[i];
			if (curKV->Key == key)
			{
				return curKV->Value;
			}
		}
		return NULL;
	}

	/**
	 * @brief Adds a key value pair to dictionary.
	 * @param key The key.
	 * @param value The Value.
	 */
	void Add(const TString& key,T value)
	{
		int Page = MOD32(key.Data[0]);
		TKeyValue<TString, T>* newKV = new TKeyValue<TString, T>(key, value);
		Map[Page].Values.Add(newKV);
		totalValues++;
	}
};

template <class T>
class TDictionaryEnumerator: public TEnumerator< TKeyValue<TString,T>* >
{
public:

	TDictionary<T>* d;
	int curMap;
	int curItem;

	inline TDictionaryEnumerator( TDictionary<T>* dict ) 
	{
		d = dict;
		Reset();
	}

	inline void Reset()
	{
		curMap = 0;
		curItem = 0;
	}

	inline bool MoveNext()
	{
		sfunc:
		if ( d->Map[curMap].Values.Count > curItem )
		{
			Current = d->Map[curMap].Values.Item[curItem];
			curItem++;
		}
		else
		{
			curMap++;
			if (curMap > 31)
			{
				curMap = 31; // in case of some idiot continue looping this will prevent crashing
				return false;
			}
			goto sfunc;
		}

		return true;
	}
};

#endif