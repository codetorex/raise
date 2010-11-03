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
	* Set enumerator to first item.
	*/
	virtual void Reset() = 0;

	/**
	* Advance enumerator to next item.
	*/
	virtual bool MoveNext() = 0;

	/**
	* Get current enumerator item.
	*/
	virtual T* Current() = 0;
};

// IEnumerable Interface

#endif