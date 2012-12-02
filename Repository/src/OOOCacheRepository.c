#include "OOOCacheRepository.h"
#include "OOOError.h"
#include "OOOCacheManifest.h"

#define MANIFEST_FILE	"MANIFEST"

/*
 * Private CacheSetManifestReadFile class declaration
 */

#define OOOClass CacheSetManifestReadFile
OOODeclarePrivate(OOOCacheRepository * pRepository, OOOCacheManifest * pCacheManifest, OOOICacheData * iCacheData)
	OOOImplements
		OOOImplement(OOOIFileReadData)
	OOOImplementsEnd
	OOOExports
	OOOExportsEnd
OOODeclareEnd

OOOPCDeclare(OOOICacheData *, getCacheData)
OOOPCDeclare(OOOCacheManifest *, getManifest)
#undef OOOClass

/*
 * Private CacheSetManifestInitializeFile class declaration
 */

#define OOOClass CacheSetManifestInitializeFile
OOODeclarePrivate(OOOCacheRepository * pRepository, OOOCacheManifest * pCacheManifest, OOOICacheData * iCacheData)
	OOOImplements
		OOOImplement(OOOIFileWriteData)
	OOOImplementsEnd
	OOOExports
	OOOExportsEnd
OOODeclareEnd

OOOPCDeclare(OOOICacheData *, getCacheData)
OOOPCDeclare(OOOCacheManifest *, getManifest)
#undef OOOClass

/*
 * Private CacheSetManifestUpdateFile class declaration
 */

#define OOOClass CacheSetManifestUpdateFile
OOODeclarePrivate(OOOCacheRepository * pRepository, OOOCacheManifest * pCacheManifest, OOOICacheData * iCacheData)
	OOOImplements
		OOOImplement(OOOIFileWriteData)
	OOOImplementsEnd
	OOOExports
	OOOExportsEnd
OOODeclareEnd

OOOPCDeclare(OOOICacheData *, getCacheData)
OOOPCDeclare(OOOCacheManifest *, getManifest)
#undef OOOClass

/*
 * Private CacheSetWriteFile class declaration
 */

#define OOOClass CacheSetWriteFile
OOODeclarePrivate(OOOCacheRepository * pRepository, OOOCacheManifest * pCacheManifest, OOOICacheData * iCacheData)
	OOOImplements
		OOOImplement(OOOIFileWriteData)
	OOOImplementsEnd
	OOOExports
	OOOExportsEnd
OOODeclareEnd

OOOPCDeclare(OOOICacheData *, getCacheData)
OOOPCDeclare(OOOCacheManifest *, getManifest)
#undef OOOClass

/*
 * Private CacheSetResetDirectory class declaration
 */

#define OOOClass CacheSetResetDirectory
OOODeclarePrivate(OOOCacheRepository * pRepository, OOOCacheManifest * pCacheManifest, OOOICacheData * iCacheData)
	OOOImplements
		OOOImplement(OOOIDirectoryRemoveData)
	OOOImplementsEnd
	OOOExports
	OOOExportsEnd
OOODeclareEnd

OOOPCDeclare(OOOCacheManifest *, getManifest)
OOOPCDeclare(OOOICacheData *, getCacheData)
#undef OOOClass

/*
 * OOOCacheRepository class implementation
 */

#define OOOClass OOOCacheRepository
typedef struct _Entry Entry;
struct _Entry
{
	char * szName;
	unsigned char * pData;
	size_t uSize;
	Entry * pNext;
};

OOOPrivateData
	OOOILog * iLog;
	OOOIFileSystem * iFileSystem;
	char * szDirectory;
	size_t uSize;
	Entry * pEntries;
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
		O_free(pLast);
	}
}
OOODestructorEnd

OOOMethod(char *, getDirectory)
{
	return OOOF(szDirectory);
}
OOOMethodEnd

OOOMethod(void, cacheSetManifestUpdated, CacheSetManifestUpdateFile * pCacheSetManifestUpdateFile, OOOIError * iError)
{
	OOOCacheManifest * pCacheManifest = OOOPCCall(CacheSetManifestUpdateFile, pCacheSetManifestUpdateFile, getManifest);
	OOOICacheData * iCacheData = OOOPCCall(CacheSetManifestUpdateFile, pCacheSetManifestUpdateFile, getCacheData);
	OOODestroy(pCacheSetManifestUpdateFile);
	OOODestroy(pCacheManifest);
	if (iError)
	{
		/*
		 * TODO: should we clean up the cache here
		 */
		OOOICall(iCacheData, cached, iError);
	}
	else
	{
		Entry * pEntry = O_malloc(sizeof(Entry));
		pEntry->szName = O_strdup(OOOICall(iCacheData, getName));
		pEntry->pData = OOOICall(iCacheData, getData);
		pEntry->uSize = OOOICall(iCacheData, getSize);
		pEntry->pNext = OOOF(pEntries);
		OOOF(pEntries) = pEntry;
		OOOICall(iCacheData, cached, NULL);
	}
}
OOOMethodEnd

OOOMethod(void, cacheSetFileWritten, CacheSetWriteFile * pCacheSetWriteFile, OOOIError * iError)
{
	OOOCacheManifest * pCacheManifest = OOOPCCall(CacheSetWriteFile, pCacheSetWriteFile, getManifest);
	OOOICacheData * iCacheData = OOOPCCall(CacheSetWriteFile, pCacheSetWriteFile, getCacheData);
	OOODestroy(pCacheSetWriteFile);
	if (iError)
	{
		OOODestroy(pCacheManifest);
		OOOICall(iCacheData, cached, iError);
	}
	else
	{
		/*
		 * Update the manifest
		 */
		OOOCall(pCacheManifest, add, OOOICall(iCacheData, getName), OOOICall(iCacheData, getSize));
		{
			CacheSetManifestUpdateFile * pCacheSetManifestUpdateFile = OOOConstruct(CacheSetManifestUpdateFile, OOOThis, pCacheManifest, iCacheData);
			OOOICall(OOOF(iFileSystem), writeFile, OOOCast(OOOIFileWriteData, pCacheSetManifestUpdateFile));
		}
	}
}
OOOMethodEnd

OOOMethod(void, updateCache, OOOCacheManifest * pCacheManifest, OOOICacheData * iCacheData)
{
	/*
	 * check the total size logged in the manifest
	 * against the cache size to see if the new data will
	 * fit - if not we will delete old files until it does
	 */
	size_t expectedSize = OOOICall(iCacheData, getSize) + OOOCall(pCacheManifest, getSize);
	if (expectedSize > OOOF(uSize))
	{
		/*
		 * TODO: remove oldest file from the cache and try again
		 */
	}
	else
	{
		/*
		 * Write the data to the cache directory
		 */
		CacheSetWriteFile * pCacheSetWriteFile = OOOConstruct(CacheSetWriteFile, OOOThis, pCacheManifest, iCacheData);
		OOOICall(OOOF(iFileSystem), writeFile, OOOCast(OOOIFileWriteData, pCacheSetWriteFile));
	}
}
OOOMethodEnd

OOOMethod(void, cacheSetManifestInitialized, CacheSetManifestInitializeFile * pCacheSetManifestInitializeFile, OOOIError * iError)
{
	OOOCacheManifest * pCacheManifest = OOOPCCall(CacheSetManifestInitializeFile, pCacheSetManifestInitializeFile, getManifest);
	OOOICacheData * iCacheData = OOOPCCall(CacheSetManifestInitializeFile, pCacheSetManifestInitializeFile, getCacheData);
	OOODestroy(pCacheSetManifestInitializeFile);
	if (iError)
	{
		OOODestroy(pCacheManifest);
		OOOICall(iCacheData, cached, iError);
	}
	else
	{
		OOOC(updateCache, pCacheManifest, iCacheData);
	}
}
OOOMethodEnd

OOOMethod(void, cacheSetDirectoryReset, CacheSetResetDirectory * pCacheSetResetDirectory, OOOIError * iError)
{
	OOOCacheManifest * pCacheManifest = OOOPCCall(CacheSetResetDirectory, pCacheSetResetDirectory, getManifest);
	OOOICacheData * iCacheData = OOOPCCall(CacheSetResetDirectory, pCacheSetResetDirectory, getCacheData);
	OOODestroy(pCacheSetResetDirectory);

	/*
	 * Ignore errors from the directory removal - might
	 * just mean the directory did not exist. If there is
	 * a real problem then there will be a problem creating
	 * the manifest file and we can catch that
	 */
	{
		CacheSetManifestInitializeFile * pCacheSetManifestInitializeFile = OOOConstruct(CacheSetManifestInitializeFile, OOOThis, pCacheManifest, iCacheData);
		OOOICall(OOOF(iFileSystem), writeFile, OOOCast(OOOIFileWriteData, pCacheSetManifestInitializeFile));
	}
}
OOOMethodEnd

OOOMethod(void, cacheSetManifestRead, CacheSetManifestReadFile * pCacheSetManifestReadFile, OOOIError * iError)
{
	OOOCacheManifest * pCacheManifest = OOOPCCall(CacheSetManifestReadFile, pCacheSetManifestReadFile, getManifest);
	OOOICacheData * iCacheData = OOOPCCall(CacheSetManifestReadFile, pCacheSetManifestReadFile, getCacheData);
	OOODestroy(pCacheSetManifestReadFile);
	if (iError)
	{
		/*
		 * Manifest could not be read so delete
		 * the cache directory if it exists and create
		 * a new manifest before continuing
		 */
		CacheSetResetDirectory * pCacheSetResetDirectory = OOOConstruct(CacheSetResetDirectory, OOOThis, pCacheManifest, iCacheData);
		OOOICall(OOOF(iLog), print, "OOOCacheRepository: Error encountered reading manifest so resetting cache: %s\n", OOOICall(iError, toString));
		OOOICall(OOOF(iFileSystem), removeDirectory, OOOCast(OOOIDirectoryRemoveData, pCacheSetResetDirectory));
	}
	else
	{
		OOOC(updateCache, pCacheManifest, iCacheData);
	}
}
OOOMethodEnd

OOOMethod(void, set, OOOICacheData * iCacheData)
{
	if (OOOICall(iCacheData, getSize) > OOOF(uSize))
	{
		OOOError * pError = OOOConstruct(OOOError, "CACHE TOO SMALL");
		OOOICall(iCacheData, cached, OOOCast(OOOIError, pError));
		OOODestroy(pError);
	}
	else
	{
		if (OOOICall(iCacheData, getName))
		{
			/*
			 * Read the cache manifest
			 */
			OOOCacheManifest * pCacheManifest = OOOConstruct(OOOCacheManifest);
			CacheSetManifestReadFile * pCacheSetManifestReadFile = OOOConstruct(CacheSetManifestReadFile, OOOThis, pCacheManifest, iCacheData);
			OOOICall(OOOF(iFileSystem), readFile, OOOCast(OOOIFileReadData, pCacheSetManifestReadFile));
		}
		else
		{
			OOOError * pError = OOOConstruct(OOOError, "INVALID NAME");
			OOOICall(iCacheData, cached, OOOCast(OOOIError, pError));
			OOODestroy(pError);
		}
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
				pEntry->pData,
				pEntry->uSize
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

OOOConstructor(OOOILog * iLog, OOOIFileSystem * iFileSystem, char * szDirectory, size_t uSize)
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

	OOOF(iLog) = iLog;
	OOOF(iFileSystem) = iFileSystem;
	OOOF(szDirectory) = szDirectory;
	OOOF(uSize) = uSize;
}
OOOConstructorEnd
#undef OOOClass

/*
 * Private CacheSetManifestReadFile class implementation
 */

#define OOOClass CacheSetManifestReadFile
OOOPrivateData
	OOOCacheRepository * pRepository;
	char * szPath;
	OOOCacheManifest * pCacheManifest;
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

OOOMethod(void, read, OOOIError * iError, unsigned char * pData, size_t uSize)
{
	if (!iError)
	{
		OOOCall(OOOF(pCacheManifest), deserialize, pData, uSize);
	}
	OOOPCCall(OOOCacheRepository, OOOF(pRepository), cacheSetManifestRead, OOOThis, iError);
}
OOOMethodEnd

OOOMethodDeclared(OOOICacheData *, getCacheData)
{
	return OOOF(iCacheData);
}
OOOMethodEnd

OOOMethodDeclared(OOOCacheManifest *, getManifest)
{
	return OOOF(pCacheManifest);
}
OOOMethodEnd

OOOConstructorPrivate(OOOCacheRepository * pRepository, OOOCacheManifest * pCacheManifest, OOOICacheData * iCacheData)
{
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
	OOOF(pCacheManifest) = pCacheManifest;
	OOOF(iCacheData) = iCacheData;
}
OOOConstructorEnd
#undef OOOClass

/*
 * Private CacheSetManifestInitializeFile class implementation
 */

#define OOOClass CacheSetManifestInitializeFile
OOOPrivateData
	OOOCacheRepository * pRepository;
	char * szPath;
	OOOCacheManifest * pCacheManifest;
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

OOOMethod(unsigned char *, getData)
{
	return OOOCall(OOOF(pCacheManifest), getSerializedData);
}
OOOMethodEnd

OOOMethod(size_t, getSize)
{
	return OOOCall(OOOF(pCacheManifest), getSerializedSize);
}
OOOMethodEnd

OOOMethod(void, written, OOOIError * iError)
{
	OOOPCCall(OOOCacheRepository, OOOF(pRepository), cacheSetManifestInitialized, OOOThis, iError);
}
OOOMethodEnd

OOOMethodDeclared(OOOICacheData *, getCacheData)
{
	return OOOF(iCacheData);
}
OOOMethodEnd

OOOMethodDeclared(OOOCacheManifest *, getManifest)
{
	return OOOF(pCacheManifest);
}
OOOMethodEnd

OOOConstructorPrivate(OOOCacheRepository * pRepository, OOOCacheManifest * pCacheManifest, OOOICacheData * iCacheData)
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
		MANIFEST_FILE
	);
	OOOF(pRepository) = pRepository;
	OOOF(pCacheManifest) = pCacheManifest;
	OOOF(iCacheData) = iCacheData;
}
OOOConstructorEnd
#undef OOOClass

/*
 * Private CacheSetManifestUpdateFile class implementation
 */

#define OOOClass CacheSetManifestUpdateFile
OOOPrivateData
	OOOCacheRepository * pRepository;
	char * szPath;
	OOOCacheManifest * pCacheManifest;
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

OOOMethod(unsigned char *, getData)
{
	return OOOCall(OOOF(pCacheManifest), getSerializedData);
}
OOOMethodEnd

OOOMethod(size_t, getSize)
{
	return OOOCall(OOOF(pCacheManifest), getSerializedSize);
}
OOOMethodEnd

OOOMethod(void, written, OOOIError * iError)
{
	OOOPCCall(OOOCacheRepository, OOOF(pRepository), cacheSetManifestUpdated, OOOThis, iError);
}
OOOMethodEnd

OOOMethodDeclared(OOOICacheData *, getCacheData)
{
	return OOOF(iCacheData);
}
OOOMethodEnd

OOOMethodDeclared(OOOCacheManifest *, getManifest)
{
	return OOOF(pCacheManifest);
}
OOOMethodEnd

OOOConstructorPrivate(OOOCacheRepository * pRepository, OOOCacheManifest * pCacheManifest, OOOICacheData * iCacheData)
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
		MANIFEST_FILE
	);
	OOOF(pRepository) = pRepository;
	OOOF(pCacheManifest) = pCacheManifest;
	OOOF(iCacheData) = iCacheData;
}
OOOConstructorEnd
#undef OOOClass

/*
 * Private CacheSetWriteFile class implementation
 */

#define OOOClass CacheSetWriteFile
OOOPrivateData
	OOOCacheRepository * pRepository;
	char * szPath;
	OOOCacheManifest * pCacheManifest;
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

OOOMethod(unsigned char *, getData)
{
	return OOOICall(OOOF(iCacheData), getData);
}
OOOMethodEnd

OOOMethod(size_t, getSize)
{
	return OOOICall(OOOF(iCacheData), getSize);
}
OOOMethodEnd

OOOMethod(void, written, OOOIError * iError)
{
	OOOPCCall(OOOCacheRepository, OOOF(pRepository), cacheSetFileWritten, OOOThis, iError);
}
OOOMethodEnd

OOOMethodDeclared(OOOCacheManifest *, getManifest)
{
	return OOOF(pCacheManifest);
}
OOOMethodEnd

OOOMethodDeclared(OOOICacheData *, getCacheData)
{
	return OOOF(iCacheData);
}
OOOMethodEnd

OOOConstructorPrivate(OOOCacheRepository * pRepository, OOOCacheManifest * pCacheManifest, OOOICacheData * iCacheData)
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
	OOOF(pCacheManifest) = pCacheManifest;
	OOOF(iCacheData) = iCacheData;
}
OOOConstructorEnd
#undef OOOClass

/*
 * Private CacheSetResetDirectory class implementation
 */

#define OOOClass CacheSetResetDirectory
OOOPrivateData
	OOOCacheRepository * pRepository;
	OOOCacheManifest * pCacheManifest;
	OOOICacheData * iCacheData;
OOOPrivateDataEnd

OOODestructor
OOODestructorEnd

OOOMethod(char *, getPath)
{
	return OOOPCCall(OOOCacheRepository, OOOF(pRepository), getDirectory);
}
OOOMethodEnd

OOOMethod(void, removed, OOOIError * iError)
{
	OOOPCCall(OOOCacheRepository, OOOF(pRepository), cacheSetDirectoryReset, OOOThis, iError);
}
OOOMethodEnd

OOOMethodDeclared(OOOCacheManifest *, getManifest)
{
	return OOOF(pCacheManifest);
}
OOOMethodEnd

OOOMethodDeclared(OOOICacheData *, getCacheData)
{
	return OOOF(iCacheData);
}
OOOMethodEnd

OOOConstructorPrivate(OOOCacheRepository * pRepository, OOOCacheManifest * pCacheManifest, OOOICacheData * iCacheData)
{
#define OOOInterface OOOIDirectoryRemoveData
	OOOMapVirtuals
		OOOMapVirtual(getPath)
		OOOMapVirtual(removed)
	OOOMapVirtualsEnd
#undef OOOInterface

	OOOMapMethods
	OOOMapMethodsEnd

	OOOF(pRepository) = pRepository;
	OOOF(pCacheManifest) = pCacheManifest;
	OOOF(iCacheData) = iCacheData;
}
OOOConstructorEnd
#undef OOOClass
