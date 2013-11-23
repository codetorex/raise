#ifndef TEST_POINTER_H
#define	TEST_POINTER_H

#include "ttest.h"
#include "tsharedpointer.h"


using namespace raise;

class TestPointer: public UnitTest
{
public:

	TestPointer(): UnitTest("Test Pointer") { }

    void Execute()
    {
		Should("Able to acquire from normal pointer");
		{
			int* testPtr = new int;
			SharedPointer<int> test1 = testPtr;
			
			Result( test1.Pointer == testPtr );
			Result( test1.Counter != 0 );
			Result( test1.Counter->Count == 1 );
		}
		
		Should("Able to acquire from another shared pointer");
		{
			int* testPtr = new int;
			SharedPointer<int> test1 = testPtr;
			SharedPointer<int> test2 = test1;
			
			Result( test1.Pointer == testPtr );
			Result( test2.Pointer == testPtr );
			Result( test1.Counter != 0 );
			Result( test2.Counter != 0 );
			Result( test2.Counter->Count == 2 );
			Result( test1.Counter == test2.Counter );
			
			Should("Setting value of one pointer change other ones");
			{
				*test2 = 100;
				Result( *test1 == *test2 );
			}
			
			Should("Able to release from another acquired pointer");
			{
				test2.Release();
				Result( test1.Counter->Count == 1 );
			}
			
			Should("Release when out of scope");
			{
				SharedPointer<int> test3 = test1;
				Result( test1.Counter->Count == 2 );
			}
			
			/// Test3 is out of scope so it should released the pointer
			Result( test1.Counter->Count == 1 );
			
			Should("Delete pointer when last one is released");
			{
				test1.Release();
				Result( test1.Counter == 0 );
				Result( test1.Pointer == 0 );
				Result( *testPtr != 100 ); // assuming freeing memory changed that part
			}
		}


    }
};


#endif	/* TESTPOINTER_H */

