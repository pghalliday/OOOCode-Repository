#ifndef OOOCacheRepository_H
#define OOOCacheRepository_H

#include "OOOCode.h"
#include "OOOIRepository.h"
#include "OOOICache.h"
#include "OOODirectory.h"

#define OOOClass OOOCacheRepository
OOODeclare(OOODirectory * pDirectory)
	OOOImplements
		OOOImplement(OOOIRepository)
		OOOImplement(OOOICache)
	OOOImplementsEnd
	OOOExports
	OOOExportsEnd
OOODeclareEnd
#undef OOOClass

#endif
