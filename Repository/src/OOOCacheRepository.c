#include "OOOCacheRepository.h"
#include "OOOError.h"

#define MANIFEST_FILE	".MANIFEST"

/*
 * Private CacheFile class declaration
 */

#define OOOClass CacheFile
OOODeclarePrivate(OOOCacheRepository * pRepository, OOOICacheData * iCacheData)
	OOOImplements
		OOOImplement(OOOIFileWriteData)
	OOOImplementsEnd
	OOOExports
	OOOExportsEnd
OOODeclareEnd

OOOPCDeclare(OOOICacheData *, getCacheData)
OOOPCDeclare(unsigned char *, getData)
OOOPCDeclare(size_t, getSize)
#undef OOOClass

/*
 * Private ManifestFile class declaration
 */

#define OOOClass ManifestFile
OOODeclarePrivate(OOOCacheRepository * pRepository)
	OOOImplements
		OOOImplement(OOOIFileWriteData)
		OOOImplement(OOOIFileReadData)
	OOOImplementsEnd
	OOOExports
	OOOExportsEnd
OOODeclareEnd

OOOPCDeclare(OOOICacheData *, getCacheData)
OOOPCDeclare(void, setCacheData, OOOICacheData * iCacheData)
#undef OOOClass

/*
 * OOOCacheRepository class implementation
 */

#define OOOClass OOOCacheRepository
typedef struct _Entry Entry;
struct _Entry
{
	char * szName;
	CacheFile * pCacheFile;
	Entry * pNext;
};

OOOPrivateData
	OOOIFileSystem * iFileSystem;
	char * szDirectory;
	size_t uSize;
	Entry * pEntries;
	ManifestFile * pManifestFile;
OOOPrivateDataEnd

OOODestructor
{
	Entry * pEntry = OOOF(pEntries);
	Entry * pLast = NULL;
	while (pEntry)
	{
		pLast = pEntry;
		pEntry = pEntry->pNext;
		O_free(pLast->szName);
		OOODestroy(pLast->pCacheFile);
		O_free(pLast);
	}
}
OOODestructorEnd

OOOMethod(char *, getDirectory)
{
	return OOOF(szDirectory);
}
OOOMethodEnd

OOOMethod(void, cacheWritten, CacheFile * pCacheFile, OOOIError * iError)
{
	OOOICacheData * iCacheData = OOOPCCall(CacheFile, pCacheFile, getCacheData);
	if (iError)
	{
		OOOICall(iCacheData, cached, iError);
		OOODestroy(pCacheFile);
	}
	else
	{
		Entry * pEntry = O_malloc(sizeof(Entry));
		pEntry->szName = O_strdup(OOOICall(iCacheData, getName));
		pEntry->pCacheFile = pCacheFile;
		pEntry->pNext = OOOF(pEntries);
		OOOF(pEntries) = pEntry;
		OOOICall(iCacheData, cached, NULL);
	}
}
OOOMethodEnd

OOOMethod(void, set, OOOICacheData * iCacheData)
{
	/*
	 * TODO: Read the cache manifest
	 */

	if (OOOICall(iCacheData, getName))
	{
		CacheFile * pCacheFile = OOOConstruct(CacheFile, OOOThis, iCacheData);
		OOOICall(OOOF(iFileSystem), writeFile, OOOCast(OOOIFileWriteData, pCacheFile));
	}
	else
	{
		OOOError * pError = OOOConstruct(OOOError, "INVALID NAME");
		OOOICall(iCacheData, cached, OOOCast(OOOIError, pError));
		OOODestroy(pError);
	}
}
OOOMethodEnd

OOOMethod(void, get, OOOIRepositoryData * iRepositoryData)
{
	char * szName = OOOICall(iRepositoryData, getName);
	Entry * pEntry = OOOF(pEntries);
	while (pEntry)
	{
		if (O_strcmp(pEntry->szName, szName) == 0)
		{
			OOOICall
			(
				iRepositoryData,
				data,
				NULL,
				OOOPCCall(CacheFile, pEntry->pCacheFile, getData),
				OOOPCCall(CacheFile, pEntry->pCacheFile, getSize)
			);
			break;
		}
		pEntry = pEntry->pNext;
	}
	if (!pEntry)
	{
		OOOError * pError = OOOConstruct(OOOError, "UNKNOWN MODULE");
		OOOICall(iRepositoryData, data, OOOCast(OOOIError, pError), NULL, 0);
		OOODestroy(pError);
	}
}
OOOMethodEnd

OOOMethod(void, manifestWritten, OOOIError * iError)
{
}
OOOMethodEnd

OOOMethod(void, manifestRead, OOOIError * iError)
{
}
OOOMethodEnd

OOOConstructor(OOOIFileSystem * iFileSystem, char * szDirectory, size_t uSize)
{
#define OOOInterface OOOICache
	OOOMapVirtuals
		OOOMapVirtual(set)
	OOOMapVirtualsEnd
#undef OOOInterface

#define OOOInterface OOOIRepository
	OOOMapVirtuals
		OOOMapVirtual(get)
	OOOMapVirtualsEnd
#undef OOOInterface

	OOOMapMethods
	OOOMapMethodsEnd

	OOOF(iFileSystem) = iFileSystem;
	OOOF(szDirectory) = szDirectory;
	OOOF(uSize) = uSize;
	OOOF(pManifestFile) = OOOConstruct(ManifestFile, OOOThis);
}
OOOConstructorEnd
#undef OOOClass

/*
 * Private CacheFile class implementation
 */

#define OOOClass CacheFile
OOOPrivateData
	OOOCacheRepository * pRepository;
	char * szPath;
	unsigned char * pData;
	size_t uSize;
	OOOICacheData * iCacheData;
OOOPrivateDataEnd

OOODestructor
{
	O_free(OOOF(szPath));
}
OOODestructorEnd

OOOMethod(char *, getPath)
{
	return OOOF(szPath);
}
OOOMethodEnd

OOOMethodDeclared(unsigned char *, getData)
{
	return OOOF(pData);
}
OOOMethodEnd

OOOMethodDeclared(size_t, getSize)
{
	return OOOF(uSize);
}
OOOMethodEnd

OOOMethod(void, written, OOOIError * iError)
{
	OOOPCCall(OOOCacheRepository, OOOF(pRepository), cacheWritten, OOOThis, iError);
}
OOOMethodEnd

OOOMethodDeclared(OOOICacheData *, getCacheData)
{
	return OOOF(iCacheData);
}
OOOMethodEnd

OOOConstructorPrivate(OOOCacheRepository * pRepository, OOOICacheData * iCacheData)
{
#define OOOInterface OOOIFileWriteData
	OOOMapVirtuals
		OOOMapVirtual(getPath)
		OOOMapVirtual(getData)
		OOOMapVirtual(getSize)
		OOOMapVirtual(written)
	OOOMapVirtualsEnd
#undef OOOInterface

	OOOMapMethods
	OOOMapMethodsEnd

	OOOF(szPath) = O_dsprintf
	(
		"%s/%s",
		OOOPCCall(OOOCacheRepository, pRepository, getDirectory),
		OOOICall(iCacheData, getName)
	);
	OOOF(pRepository) = pRepository;
	OOOF(iCacheData) = iCacheData;
}
OOOConstructorEnd
#undef OOOClass

/*
 * Private ManifestFile class implementation
 */

#define OOOClass ManifestFile
OOOPrivateData
	OOOCacheRepository * pRepository;
	char * szPath;
	char * szManifest;
	OOOICacheData * iCacheData;
OOOPrivateDataEnd

OOODestructor
{
	if (OOOF(szManifest))
	{
		O_free(OOOF(szManifest));
	}
	O_free(OOOF(szPath));
}
OOODestructorEnd

OOOMethod(char *, getPath)
{
	return OOOF(szPath);
}
OOOMethodEnd

OOOMethod(unsigned char *, getData)
{
	return OOOF(szManifest);
}
OOOMethodEnd

OOOMethod(size_t, getSize)
{
	return O_strlen(OOOF(szManifest) + 1);
}
OOOMethodEnd

OOOMethod(void, written, OOOIError * iError)
{
	OOOPCCall(OOOCacheRepository, OOOF(pRepository), manifestWritten, iError);
}
OOOMethodEnd

OOOMethod(void, read, OOOIError * iError, unsigned char * pData, size_t uSize)
{
	OOOPCCall(OOOCacheRepository, OOOF(pRepository), manifestRead, iError);
}
OOOMethodEnd

OOOMethodDeclared(OOOICacheData *, getCacheData)
{
	return OOOF(iCacheData);
}
OOOMethodEnd

OOOMethodDeclared(void, setCacheData, OOOICacheData * iCacheData)
{
	OOOF(iCacheData) = iCacheData;
}
OOOMethodEnd

OOOConstructorPrivate(OOOCacheRepository * pRepository)
{
#define OOOInterface OOOIFileWriteData
	OOOMapVirtuals
		OOOMapVirtual(getPath)
		OOOMapVirtual(getData)
		OOOMapVirtual(getSize)
		OOOMapVirtual(written)
	OOOMapVirtualsEnd
#undef OOOInterface

#define OOOInterface OOOIFileReadData
	OOOMapVirtuals
		OOOMapVirtual(getPath)
		OOOMapVirtual(read)
	OOOMapVirtualsEnd
#undef OOOInterface

	OOOMapMethods
	OOOMapMethodsEnd

	OOOF(szPath) = O_dsprintf
	(
		"%s/%s",
		OOOPCCall(OOOCacheRepository, pRepository, getDirectory),
		MANIFEST_FILE
	);
	OOOF(pRepository) = pRepository;
}
OOOConstructorEnd
#undef OOOClass
