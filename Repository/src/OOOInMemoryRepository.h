#ifndef OOOInMemoryRepository_H
#define OOOInMemoryRepository_H

#include "OOOCode.h"
#include "OOOIRepository.h"

#define OOOClass OOOInMemoryRepository
OOODeclare()
	OOOImplements
		OOOImplement(OOOIRepository)
	OOOImplementsEnd
	OOOExports
		OOOExport(void, add, char * szName, unsigned char * pData, size_t uSize)
	OOOExportsEnd
OOODeclareEnd
#undef OOOClass

#endif
