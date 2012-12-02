#ifndef OOOCacheManifest_H
#define OOOCacheManifest_H

#include "OOOCode.h"

#define OOOClass OOOCacheManifest
OOODeclare()
	OOOImplements
	OOOImplementsEnd
	OOOExports
		OOOExport(void, deserialize, unsigned char * pData, size_t uSize)
		OOOExport(unsigned char *, getSerializedData)
		OOOExport(size_t, getSerializedSize)
		OOOExport(size_t, getSize)
		OOOExport(char *, getOldest)
		OOOExport(void, add, char * szFile, size_t uSize)
		OOOExport(void, remove, char * szFile)
	OOOExportsEnd
OOODeclareEnd
#undef OOOClass

#endif
