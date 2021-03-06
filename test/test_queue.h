#ifndef TEST_QUEUE_H
#define TEST_QUEUE_H

#include "ttest.h"
#include "tqueue.h"
#include "mmathdriver.h"

class TestTQueue: public UnitTest
{
public:
	TestTQueue(): UnitTest("TQueue check") 
	{

	}

	void Execute()
	{
		TQueue<int> tq;
		TQueue<int> tq2;

		int testary[] = {5,4,3,2,1};
		int testary2[5];

		for (int i=0;i<5;i++)
		{
			tq.Enqueue(testary[i]);
		}
		tq2.Enqueue(testary,5);

		Result(tq.Count == 5, "Item enqueue count");
		Result(tq.Head == 0, "Item enqueue head");
		Result(tq.Tail == 5, "Item enqueue tail");

		Result(tq.Peek() == 5, "Item peek");

		for (int i=0;i<4;i++)
		{
			Result( testary[i] == tq.Dequeue(),"Item enqueue/dequeue no " + Convert::ToString(i));
		}
		tq2.Dequeue(testary2,4);
		
		Result(tq.Head == 4, "Item dequeue head");
		Result(tq.Tail == 5, "Item dequeue tail");

		for (int i=0;i<5;i++)
		{
			tq.Enqueue(testary[i]);
		}
		tq2.Enqueue(testary,5);

		Result(tq.Head == 4, "Item re enqueue head");
		Result(tq.Tail == 2, "Item re enqueue tail");

		Result(tq.Head == tq2.Head, "Enqueue array and enqueue comparison head");
		Result(tq.Tail == tq2.Tail, "Enqueue array and enqueue comparison tail");

		Result(tq.Dequeue() == 1, "Item dequeue from old value");

		for (int i=0;i<5;i++)
		{
			Result( testary[i] == tq.Dequeue(),"Item re enqueue/dequeue no " + Convert::ToString(i));
		}

		/**
		 * ByteQueue is very important structure for networking.
		 * So this test will try to simulate it.
		 * Its very important to make this component work as expected.
		 * So it will be easier to debug network stuff.
		 */

		// Now generate a large data like 4 mb then array enqueue / dequeue and compare results
		ui32 DataSize = 4 * MB;
		
		Array<byte> bytes(DataSize);
		for (int i=0;i< DataSize;i++)
		{
			byte rnd = Math.Random() % 256;
			bytes.UnsafeAdd(rnd);
		}

		TByteQueue bq;

		int enindex = 0;
		int deindex = 0;
		byte tmp[128];

		bool passed = true;



		while(deindex < DataSize)
		{
			if (enindex < DataSize)
			{
				ui32 rnden = 128 + (Math.Random() % 128);
				if (enindex + rnden > DataSize)
				{
					rnden = DataSize - enindex;
				}
				bq.Enqueue(bytes.Items + enindex, rnden);
				enindex += rnden;
			}
			
			ui32 rndde = 64 + (Math.Random() % 64);
			ui32 readden = bq.Dequeue(tmp,rndde);

			int cmpresult = MemoryDriver::Compare(bytes.Items + deindex,tmp,readden);
			if (cmpresult != 0)
			{
				passed = false;
				break;
			}

			deindex += readden;
		}

		if ( deindex != enindex || enindex != DataSize || bq.Head != 0 || bq.Tail != 0 || bq.Count != 0 )
		{
			passed = false;
		}

		TStringBuilderStack<512> sb;
		sb.Append("EnI: ");
		sb.Append(enindex);
		sb.Append(" DeIx: ");
		sb.Append(deindex);
		sb.Append(" H: ");
		sb.Append(bq.Head);
		sb.Append(" T: ");
		sb.Append(bq.Tail);
		sb.Append(" C: ");
		sb.Append(bq.Count);
		sb.Append(" Cp: ");
		sb.Append(bq.Capacity);

		if (!passed)
		{
			Result(passed, "ByteQueue extensive usage test" + sb.ToString());
		}
		else
		{
			Result(passed, "ByteQueue extensive usage test");
			Output("After extensive usage:" );
			Output(sb.ToString());
		}
	}
} TQueueCheck;

#endif