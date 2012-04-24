#ifndef TENUMERATOR_H
#define TENUMERATOR_H

/**
* Enumerator interface...
*/
template<class T>
class TEnumerator
{
public:
	/**
	 * @brief Set enumerator to first item.
	 */
	virtual void Reset() = 0;

	/**
	 * @brief Advance enumerator to next item.
	 */
	virtual bool MoveNext() = 0;

	// TODO: this could be reference righT?
	/// Current item
	T Current;
};


template<class T>
class TEnumerable
{
public:
	virtual T GetEnumerator() = 0;
};

#endif