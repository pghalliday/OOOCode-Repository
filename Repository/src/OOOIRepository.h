#ifndef OOOIRepository_H
#define OOOIRepository_H

#include "OOOCode.h"
#include "OOOIRepositoryData.h"

#define OOOInterface OOOIRepository
OOOVirtuals
	OOOVirtual(void, get, OOOIRepositoryData * iRepositoryData)
OOOVirtualsEnd
#undef OOOInterface

#endif
