#ifndef OOOIFile_H
#define OOOIFile_H

#include "OOOCode.h"
#include "OOOIFileReadData.h"
#include "OOOIFileWriteData.h"

#define OOOInterface OOOIFile
OOOVirtuals
	OOOVirtual(void, read, OOOIFileReadData * iFileReadData)
	OOOVirtual(void, write, OOOIFileWriteData * iFileWriteData)
OOOVirtualsEnd
#undef OOOInterface

#endif
