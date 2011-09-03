#include "stdafx.h"
#include "tencoding.h"
#include "tutf8encoding.h"
#include "tutf16encoding.h"
#include "tlatin1encoding.h"

UTF8Encoding& Encoding::UTF8 = UTF8Encoding();
UTF16Encoding& Encoding::UTF16 = UTF16Encoding();
TLatin1Encoding& Encoding::Latin1 = TLatin1Encoding();

