#include "stdafx.h"
#include "tstreamwriterutf8.h"

byte TStreamWriterUTF8::UTF8ByteOrderMark[] = { 0xEF , 0xBB ,0xBF };
int TStreamWriterUTF8::UTF8ByteOrderMarkLength = 3;
