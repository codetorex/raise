#include "stdafx.h"
#include "tencoding.h"


TEncoding* TEncoding::ASCII   = new TEncoding("ASCII",8,8,20127);
TEncoding* TEncoding::Unicode = new TEncoding("Unicode (UTF-16)",16,32,1200);
TEncoding* TEncoding::UTF8    = new TEncoding("Unicode (UTF-8)",8,32,65001);

