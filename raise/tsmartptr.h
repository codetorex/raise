#ifndef TSMARTPTR_H
#define TSMARTPTR_H
/*

template <class T> class ptr
{
	T* pointer;
	unsigned int* counter;
public:
	ptr();
	ptr(T* pointer);
	ptr(const ptr<T>& copy);

	~ptr();

	ptr<T>& operator=(const ptr<T>& copy);
	operator T*();
	T* operator->();
};

template <class T> ptr<T>::ptr()
{
	pointer = nullptr;
	counter = nullptr;
}

template <class T> ptr<T>::ptr(T* pointer)
{
	this->pointer = pointer;
	counter = new unsigned int();
	*counter = 1;
}

template <class T> ptr<T>::ptr(const ptr<T>& copy)
{
	pointer = copy.pointer;
	counter = copy.counter;
	if (counter != nullptr)
	{
		(*counter)++;
	}
}

template <class T> ptr<T>::~ptr()
{
	if (counter != nullptr)
	{
		(*counter)--;
		if (*counter == 0)
		{
			delete pointer;
			delete counter;
		}
	}
}

template <class T> ptr<T>& ptr<T>::operator=(const ptr<T>& copy)
{
	if (counter != nullptr)
	{
		(*counter)--;
		if (*counter == 0)
		{
			delete pointer;
			delete counter;
		}
	}

	pointer = copy.pointer;
	counter = copy.counter;
	if (counter != nullptr)
	{
		(*counter)++;
	}

	return *this;
}

template <class T> ptr<T>::operator T*()
{
	return pointer;
}

template <class T> T* ptr<T>::operator->()
{
	return pointer;
}*/

#endif