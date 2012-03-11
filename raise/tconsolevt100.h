#ifndef TCONSOLEVT100_H
#define TCONSOLEVT100_H

#include "tconsoledriver.h"
#include "tstream.h"

class TConsoleVT100Input: public TStream // CAN BE USED FOR READING APPLICATIONS?
{
public:

	TConsoleDriver* Driver;

	// HANDLES ESCAPE SEQUENCES AND CALLS THE FUNCTIONS ACCORDINGLY?

};

class TConsoleVT100Output: public TConsoleDriver // CAN BE USED FOR WRITING APPLICATIONS
{
public:

	// CREATES ESCAPE SEQUENCES
};


#endif