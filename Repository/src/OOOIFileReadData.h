#ifndef OOOIFileReadData_H
#define OOOIFileReadData_H

#include "OOOCode.h"
#include "OOOIError.h"

#define OOOInterface OOOIFileReadData
OOOVirtuals
	OOOVirtual(void, data, OOOIError * iError, unsigned char * pData, size_t uSize)
OOOVirtualsEnd
#undef OOOInterface

#endif
