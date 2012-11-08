#ifndef OOOIRepositoryData_H
#define OOOIRepositoryData_H

#include "OOOCode.h"
#include "OOOIError.h"

#define OOOInterface OOOIRepositoryData
OOOVirtuals
	OOOVirtual(char *, getName)
	OOOVirtual(void, load, OOOIError * iError, unsigned char * pData, size_t uSize)
OOOVirtualsEnd
#undef OOOInterface

#endif
