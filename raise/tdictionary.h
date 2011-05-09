#ifndef TDICTIONARY_H
#define TDICTIONARY_H

#include "tkeyvalue.h"

template <class T>
class RDLL TDictonaryPage
{
public:
	TArray< TKeyValue<str8,T>* > Values;
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
	T Get(const str8& key)
	{
		int page = MOD32(key.Chars[0]);
		for (int i=0;i<Map[page].values.Count;i++)
		{
			TKeyValue<str8,T>* curKV = Map[page].Values.Item[i];
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
	void Add(const str8& key,T value)
	{
		int Page = MOD32(key.Chars[0]);
		TKeyValue<str8, T>* newKV = new TKeyValue<str8, T>(key, value);
		Map[Page].Values.Add(newKV);
		totalValues++;
	}
};

#endif