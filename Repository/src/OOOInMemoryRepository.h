#ifndef OOOInMemoryRepository_H
#define OOOInMemoryRepository_H

#include "OOOCode.h"
#include "OOOIRepository.h"
#include "OOOICache.h"

#define OOOClass OOOInMemoryRepository
OOODeclare(OOOICache * iCache)
	OOOImplements
		OOOImplement(OOOIRepository)
	OOOImplementsEnd
	OOOExports
		OOOExport(void, add, char * szName, unsigned char * pData, size_t uSize)
	OOOExportsEnd
OOODeclareEnd
#undef OOOClass

#endif
