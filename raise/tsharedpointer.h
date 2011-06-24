#ifndef TSHAREDPOINTER_H
#define TSHAREDPOINTER_H

template <class T>
class TSharedPointer
{
public:
	T* Ptr;
	int* Reference;

	TSharedPointer()
	{
		Ptr = 0;
		Reference = 0;
	}

	TSharedPointer(const T* _ptr)
	{
		this = _ptr;
	}

	TSharedPointer( const TSharedPointer<T>& val)
	{
		Ptr = val.Ptr;
		Reference = val.Reference;
	}

	~TSharedPointer()
	{
		delete Ptr;
		delete Reference;
	}

	TSharedPointer& operator = (const T* val)
	{
		Ptr = val;
		Reference = new int;
		*Reference = 1;
	}

	void Get()
	{
		*(Reference++);
		return Ptr;
	}

	void Release()
	{
		*(Reference--);
		if (Reference == 0)
		{
			delete ptr;
		}
	}
};

#endif