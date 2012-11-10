#ifndef OOOCacheRepository_H
#define OOOCacheRepository_H

#include "OOOCode.h"
#include "OOOIRepository.h"
#include "OOOICache.h"

#define OOOClass OOOCacheRepository
OOODeclare()
	OOOImplements
		OOOImplement(OOOIRepository)
		OOOImplement(OOOICache)
	OOOImplementsEnd
	OOOExports
	OOOExportsEnd
OOODeclareEnd
#undef OOOClass

#endif
