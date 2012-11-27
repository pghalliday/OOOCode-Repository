#ifndef OOOIFileWriteData_H
#define OOOIFileWriteData_H

#include "OOOCode.h"
#include "OOOIError.h"

#define OOOInterface OOOIFileWriteData
OOOVirtuals
	OOOVirtual(unsigned char *, getData)
	OOOVirtual(size_t, getSize)
	OOOVirtual(void, written, OOOIError * iError)
OOOVirtualsEnd
#undef OOOInterface

#endif
