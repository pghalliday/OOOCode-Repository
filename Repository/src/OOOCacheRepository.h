#ifndef OOOCacheRepository_H
#define OOOCacheRepository_H

#include "OOOCode.h"
#include "OOOIRepository.h"
#include "OOOICache.h"
#include "OOOIFileSystem.h"
#include "OOOILog.h"

#define OOOClass OOOCacheRepository
OOODeclare(OOOILog * iLog, OOOIFileSystem * iFileSystem, char * szDirectory, size_t uSize)
	OOOImplements
		OOOImplement(OOOIRepository)
		OOOImplement(OOOICache)
	OOOImplementsEnd
	OOOExports
	OOOExportsEnd
OOODeclareEnd
#undef OOOClass

#endif
