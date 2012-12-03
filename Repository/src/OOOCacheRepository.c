#include "OOOCacheRepository.h"
#include "OOOError.h"
#include "OOOCacheManifest.h"

#define MANIFEST_FILE	"MANIFEST"

/*
 * Private CacheSetClosure class declaration
 */
#define OOOClass CacheSetClosure
OOODeclarePrivate(OOOICacheData * iCacheData, char * szDirectory)
	OOOImplements
	OOOImplementsEnd
	OOOExports
		OOOExport(OOOCacheManifest *, getCacheManifest)
		OOOExport(OOOICacheData *, getCacheData)
		OOOExport(char *, getManifestPath)
		OOOExport(char *, getFilePath)
	OOOExportsEnd
OOODeclareEnd

OOOPrivateData
	OOOICacheData * iCacheData;
	OOOCacheManifest * pCacheManifest;
	char * szManifestPath;
	char * szFilePath;
OOOPrivateDataEnd

OOODestructor
{
	OOODestroy(OOOF(pCacheManifest));
	O_free(OOOF(szManifestPath));
	O_free(OOOF(szFilePath));
}
OOODestructorEnd

OOOMethod(OOOCacheManifest *, getCacheManifest)
{
	return OOOF(pCacheManifest);
}
OOOMethodEnd

OOOMethod(OOOICacheData *, getCacheData)
{
	return OOOF(iCacheData);
}
OOOMethodEnd

OOOMethod(char *, getManifestPath)
{
	return OOOF(szManifestPath);
}
OOOMethodEnd

OOOMethod(char *, getFilePath)
{
	return OOOF(szFilePath);
}
OOOMethodEnd

OOOConstructorPrivate(OOOICacheData * iCacheData, char * szDirectory)
{
	OOOMapMethods
		OOOMapMethod(getCacheManifest)
		OOOMapMethod(getCacheData)
		OOOMapMethod(getManifestPath)
		OOOMapMethod(getFilePath)
	OOOMapMethodsEnd

	OOOF(iCacheData) = iCacheData;
	OOOF(pCacheManifest) = OOOConstruct(OOOCacheManifest);
	OOOF(szManifestPath) = O_dsprintf("%s/%s", szDirectory, MANIFEST_FILE);
	OOOF(szFilePath) = O_dsprintf("%s/%s", szDirectory, OOOICall(iCacheData, getName));
}
OOOConstructorEnd
#undef OOOClass

/*
 * OOOCacheRepository class implementation
 */
#define OOOClass OOOCacheRepository

/*
 * Private CacheSetManifestReadFile class declaration
 */
#define OOOClosure CacheSetReadFile
OOOIFileReadDataClosure(CacheSetClosure)
#undef OOOClosure

/*
 * Private CacheSetWriteFile class declaration
 */
#define OOOClosure CacheSetWriteFile
OOOIFileWriteDataClosure(CacheSetClosure)
#undef OOOClosure

/*
 * Private CacheSetResetDirectory class declaration
 */
#define OOOClosure CacheSetRemoveDirectory
OOOIDirectoryRemoveDataClosure(CacheSetClosure)
#undef OOOClosure

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

OOOMethod(void, cacheSetManifestUpdated, CacheSetClosure * pCacheSetClosure, OOOIError * iError)
{
	OOOICacheData * iCacheData = OOOPCCall(CacheSetClosure, pCacheSetClosure, getCacheData);
	OOODestroy(pCacheSetClosure);
	if (iError)
	{
		/*
		 * TODO: should we clean up the cache here?
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

OOOMethod(void, cacheSetFileWritten, CacheSetClosure * pCacheSetClosure, OOOIError * iError)
{
	OOOICacheData * iCacheData = OOOPCCall(CacheSetClosure, pCacheSetClosure, getCacheData);
	if (iError)
	{
		OOODestroy(pCacheSetClosure);
		OOOICall(iCacheData, cached, iError);
	}
	else
	{
		/*
		 * Update the manifest
		 */
		OOOCacheManifest * pCacheManifest = OOOPCCall(CacheSetClosure, pCacheSetClosure, getCacheManifest);
		OOOCall(pCacheManifest, add, OOOICall(iCacheData, getName), OOOICall(iCacheData, getSize));
		{
			CacheSetWriteFile * pCacheSetWriteFile = OOOClosureConstruct
			(
				CacheSetWriteFile,
				cacheSetManifestUpdated,
				pCacheSetClosure,
				OOOPCCall(CacheSetClosure, pCacheSetClosure, getManifestPath),
				OOOCall(pCacheManifest, getSerializedData),
				OOOCall(pCacheManifest, getSerializedSize)
			);
			OOOICall(OOOF(iFileSystem), writeFile, OOOCast(OOOIFileWriteData, pCacheSetWriteFile));
		}
	}
}
OOOMethodEnd

OOOMethod(void, updateCache, CacheSetClosure * pCacheSetClosure)
{
	/*
	 * check the total size logged in the manifest
	 * against the cache size to see if the new data will
	 * fit - if not we will delete old files until it does
	 */
	OOOICacheData * iCacheData = OOOPCCall(CacheSetClosure, pCacheSetClosure, getCacheData);
	OOOCacheManifest * pCacheManifest = OOOPCCall(CacheSetClosure, pCacheSetClosure, getCacheManifest);
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
		CacheSetWriteFile * pCacheSetWriteFile = OOOClosureConstruct
		(
			CacheSetWriteFile,
			cacheSetFileWritten,
			pCacheSetClosure,
			OOOPCCall(CacheSetClosure, pCacheSetClosure, getFilePath),
			OOOICall(iCacheData, getData),
			OOOICall(iCacheData, getSize)
		);
		OOOICall(OOOF(iFileSystem), writeFile, OOOCast(OOOIFileWriteData, pCacheSetWriteFile));
	}
}
OOOMethodEnd

OOOMethod(void, cacheSetManifestInitialized, CacheSetClosure * pCacheSetClosure, OOOIError * iError)
{
	if (iError)
	{
		OOOICacheData * iCacheData = OOOPCCall(CacheSetClosure, pCacheSetClosure, getCacheData);
		OOODestroy(pCacheSetClosure);
		OOOICall(iCacheData, cached, iError);
	}
	else
	{
		OOOC(updateCache, pCacheSetClosure);
	}
}
OOOMethodEnd

OOOMethod(void, cacheSetDirectoryReset, CacheSetClosure * pCacheSetClosure, OOOIError * iError)
{
	/*
	 * Ignore errors from the directory removal - might
	 * just mean the directory did not exist. If there is
	 * a real problem then there will be a problem creating
	 * the manifest file and we can catch that
	 */
	OOOCacheManifest * pCacheManifest = OOOPCCall(CacheSetClosure, pCacheSetClosure, getCacheManifest);
	CacheSetWriteFile * pCacheSetWriteFile = OOOClosureConstruct
	(
		CacheSetWriteFile,
		cacheSetManifestInitialized,
		pCacheSetClosure,
		OOOPCCall(CacheSetClosure, pCacheSetClosure, getManifestPath),
		OOOCall(pCacheManifest, getSerializedData),
		OOOCall(pCacheManifest, getSerializedSize)
	);
	OOOICall(OOOF(iFileSystem), writeFile, OOOCast(OOOIFileWriteData, pCacheSetWriteFile));
}
OOOMethodEnd

OOOMethod(void, cacheSetManifestRead, CacheSetClosure * pCacheSetClosure, OOOIError * iError, unsigned char * pData, size_t uSize)
{
	if (iError)
	{
		/*
		 * Manifest could not be read so delete
		 * the cache directory if it exists and create
		 * a new manifest before continuing
		 */
		CacheSetRemoveDirectory * pCacheSetRemoveDirectory = OOOClosureConstruct
		(
			CacheSetRemoveDirectory,
			cacheSetDirectoryReset,
			pCacheSetClosure,
			OOOF(szDirectory)
		);
		OOOICall(OOOF(iLog), print, "OOOCacheRepository: Error encountered reading manifest so resetting cache: %s\n", OOOICall(iError, toString));
		OOOICall(OOOF(iFileSystem), removeDirectory, OOOCast(OOOIDirectoryRemoveData, pCacheSetRemoveDirectory));
	}
	else
	{
		OOOCall(OOOPCCall(CacheSetClosure, pCacheSetClosure, getCacheManifest), deserialize, pData, uSize);
		OOOC(updateCache, pCacheSetClosure);
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
			 * Start the closure
			 */
			CacheSetClosure * pCacheSetClosure = OOOConstruct
			(
				CacheSetClosure,
				iCacheData,
				OOOF(szDirectory)
			);

			/*
			 * Read the cache manifest
			 */
			CacheSetReadFile * pCacheSetReadFile = OOOClosureConstruct
			(
				CacheSetReadFile,
				cacheSetManifestRead,
				pCacheSetClosure,
				OOOPCCall(CacheSetClosure, pCacheSetClosure, getManifestPath)
			);
			OOOICall(OOOF(iFileSystem), readFile, OOOCast(OOOIFileReadData, pCacheSetReadFile));
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
