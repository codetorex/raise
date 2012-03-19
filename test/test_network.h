#ifndef TEST_NETWORK_H
#define TEST_NETWORK_H

#include "ttest.h"
#include "nipaddress.h"

#ifdef WIN32
#pragma warning( push )
#pragma warning ( disable: 4101 )
#endif

class TestNIPAddress4: public TTestCheck
{
public:

	TestNIPAddress4(): TTestCheck("NAddress4 check"){}

	void Test()
	{
		NAddress4 TestAddress;

		TestAddress = "0.0.0.0";
		AddResult( TestAddress.Address == 0, "IP v4 assignment from string 0.0.0.0");

		TestAddress = "192.168.2.1";
		ui32 sysfunc = inet_addr("192.168.2.1");
		AddResult( TestAddress.Address == sysfunc, "IP v4 assignment from string 192.168.2.1");
		AddResult(TestAddress.A == 192, "IP v4 assignment from string a");
		AddResult(TestAddress.B == 168, "IP v4 assignment from string b");
		AddResult(TestAddress.C == 2, "IP v4 assignment from string c");
		AddResult(TestAddress.D == 1, "IP v4 assignment from string d");

		try
		{
			TestAddress = ".1.2.3";
			AddResult(false, "IP v4 assignment from wrong string exception thrown a");
		}
		catch (Exception& e)
		{
			AddResult(true, "IP v4 assignment from wrong string exception thrown a");
		}

		try
		{
			TestAddress = "0..2.1";
			AddResult(false, "IP v4 assignment from wrong string exception thrown b");
		}
		catch (Exception& e)
		{
			AddResult(true, "IP v4 assignment from wrong string exception thrown b");
		}
		
		try
		{
			TestAddress = "0.1..1";
			AddResult(false, "IP v4 assignment from wrong string exception thrown c");
		}
		catch (Exception& e)
		{
			AddResult(true, "IP v4 assignment from wrong string exception thrown c");
		}

		try
		{
			TestAddress = "0.0.0.";
			AddResult(false, "IP v4 assignment from wrong string exception thrown d");
		}
		catch (Exception& e)
		{
			AddResult(true, "IP v4 assignment from wrong string exception thrown d");
		}

		AddResult(TestAddress.ToString() == "192.168.2.1", "IP v4 ToString()");

		TestAddress = inet_addr("141.101.124.244");
		AddResult(TestAddress.A == 141, "IP v4 assignment from integer a");
		AddResult(TestAddress.B == 101, "IP v4 assignment from integer b");
		AddResult(TestAddress.C == 124, "IP v4 assignment from integer c");
		AddResult(TestAddress.D == 244, "IP v4 assignment from integer d");

		TestAddress.Set(74,125,79,113);
		AddResult(TestAddress.A == 74, "IP v4 assignment by parts a");
		AddResult(TestAddress.B == 125, "IP v4 assignment by parts b");
		AddResult(TestAddress.C == 79, "IP v4 assignment by parts c");
		AddResult(TestAddress.D == 113, "IP v4 assignment by parts d");

		AddResult(TestAddress.ToString() == "74.125.79.113", "IP v4 assignment by parts and ToString");


		try
		{
			TestAddress = "-1.0.0.0";
			AddResult(false, "IP v4 assignment from wrong string negative exception thrown a");
		}
		catch (Exception& e)
		{
			AddResult(true, "IP v4 assignment from wrong string negative exception thrown a");
		}

		try
		{
			TestAddress = "0.0.0.-1";
			AddResult(false, "IP v4 assignment from wrong string negative exception thrown d");
		}
		catch (Exception& e)
		{
			AddResult(true, "IP v4 assignment from wrong string negative exception thrown d");
		}

		try
		{
			TestAddress = "55555555.0.0.0";
			AddResult(false, "IP v4 assignment from wrong string big exception thrown a");
		}
		catch (Exception& e)
		{
			AddResult(true, "IP v4 assignment from wrong string big exception thrown a");
		}


		try
		{
			TestAddress = "564679855";
			AddResult(false, "IP v4 assignment from wrong string");
		}
		catch (Exception& e)
		{
			AddResult(true, "IP v4 assignment from wrong string");
		}
		
		try
		{
			TestAddress = "12";
			AddResult(false, "IP v4 assignment from wrong string 2");
		}
		catch (Exception& e)
		{
			AddResult(true, "IP v4 assignment from wrong string 2");
		}

		try
		{
			TestAddress = "256.256.256.256";
			AddResult(false, "IP v4 assignment from wrong string 3");
		}
		catch (Exception& e)
		{
			AddResult(true, "IP v4 assignment from wrong string 3");
		}

	}

} NIPAddress4Check;

#ifdef WIN32
#pragma warning( pop )
#endif

#endif