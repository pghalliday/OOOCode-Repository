#ifndef OOOCacheRepository_H
#define OOOCacheRepository_H

#include "OOOCode.h"
#include "OOOIRepository.h"
#include "OOOICache.h"
#include "OOOIFileSystem.h"

#define OOOClass OOOCacheRepository
OOODeclare(OOOIFileSystem * iFileSystem, char * szDirectory)
	OOOImplements
		OOOImplement(OOOIRepository)
		OOOImplement(OOOICache)
	OOOImplementsEnd
	OOOExports
	OOOExportsEnd
OOODeclareEnd
#undef OOOClass

#endif
