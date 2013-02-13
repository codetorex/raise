#ifndef TKEYVALUE_H
#define TKEYVALUE_H


/**
 * @brief Key value pair. 
 */
template <class K, class V>
class TKeyValue
{
public:
	K Key;
	V Value;

	TKeyValue()
	{

	}

	/**
	 * @brief Easy constructor.
	 * @param _key The key.
	 * @param _value The value.
	 */
	inline TKeyValue( const K& _key, const V& _value)
	{
		Key = _key;
		Value = _value;
	}
};


/**
 * @brief Key and value array pair. 
 */
template <class K, class V>
class TKeyValueArray
{
public:
	K Key;
	TArray<V> Values;

	TKeyValueArray()
	{

	}

	/**
	 * @brief Easy constructor.
	 * @param _key The key.
	 * @param _value The value.
	 */
	inline TKeyValueArray( const K& _key, const V& _value)
	{
		Key = _key;
		Values.Add(_value);
	}
};


#endif
