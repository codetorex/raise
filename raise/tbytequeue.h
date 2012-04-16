#ifndef TBYTEQUEUE_H
#define TBYTEQUEUE_H

/**
 * Good for networking stuff I guess.
 */
class TByteQueue
{
public:

};

/**
 * Very good for networking stuff.
 */
class TByteQueueLockable
{
public:
	byte*	Data;
	ui32	Capacity;
	ui32	Count;

	ui32	Head;
	ui32	Tail;

	TByteQueueLockable()
	{
		Data = 0;
		Capacity = 0;
		Count = 0;
		Head = 0;
		Tail = 0;
	}

	void Allocate(ui32 _capacity)
	{

	}

	void Enqueue(byte* data, ui32 length)
	{

	}

	void Dequeue(byte* data, ui32 length)
	{

	}


};

#endif