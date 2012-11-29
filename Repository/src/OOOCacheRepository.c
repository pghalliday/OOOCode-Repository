#include "OOOCacheRepository.h"
#include "OOOError.h"

#define OOOClass File
OOODeclarePrivate(OOOCacheRepository * pRepository, char * szDirectory, char * szFile, unsigned char * pData, size_t uSize, OOOICacheData * iCacheData)
	OOOImplements
		OOOImplement(OOOIFileWriteData)
	OOOImplementsEnd
	OOOExports
	OOOExportsEnd
OOODeclareEnd
#undef OOOClass

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
	OOOIFileSystem * iFileSystem;
	char * szDirectory;
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

OOOMethod(void, written, File * pFile, OOOIError * iError, OOOICacheData * iCacheData)
{
	OOODestroy(pFile);
	if (iError)
	{
		OOOICall(iCacheData, cached, iError);
	}
	else
	{
		char * szName = OOOICall(iCacheData, getName);
		unsigned char * pData = OOOICall(iCacheData, getData);
		size_t uSize = OOOICall(iCacheData, getSize);
		Entry * pEntry = O_malloc(sizeof(Entry));
		pEntry->szName = O_strdup(szName);
		pEntry->pData = pData;
		pEntry->uSize = uSize;
		pEntry->pNext = OOOF(pEntries);
		OOOF(pEntries) = pEntry;
		OOOICall(iCacheData, cached, NULL);
	}
}
OOOMethodEnd

OOOMethod(void, set, OOOICacheData * iCacheData)
{
	char * szName = OOOICall(iCacheData, getName);
	unsigned char * pData = OOOICall(iCacheData, getData);
	size_t uSize = OOOICall(iCacheData, getSize);

	if (szName)
	{
		File * pFile = OOOConstruct(File, OOOThis, OOOF(szDirectory), szName, pData, uSize, iCacheData);
		OOOICall(OOOF(iFileSystem), writeFile, OOOCast(OOOIFileWriteData, pFile));
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
			OOOICall(iRepositoryData, data, NULL, pEntry->pData, pEntry->uSize);
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

OOOConstructor(OOOIFileSystem * iFileSystem, char * szDirectory)
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
}
OOOConstructorEnd
#undef OOOClass

#define OOOClass File
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

OOOMethod(unsigned char *, getData)
{
	return OOOF(pData);
}
OOOMethodEnd

OOOMethod(size_t, getSize)
{
	return OOOF(uSize);
}
OOOMethodEnd

OOOMethod(void, written, OOOIError * iError)
{
	OOOPCCall(OOOCacheRepository, OOOF(pRepository), written, OOOThis, iError, OOOF(iCacheData));
}
OOOMethodEnd

OOOConstructorPrivate(OOOCacheRepository * pRepository, char * szDirectory, char * szFile, unsigned char * pData, size_t uSize, OOOICacheData * iCacheData)
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

	OOOF(szPath) = O_dsprintf("%s/%s", szDirectory, szFile);
	OOOF(pRepository) = pRepository;
	OOOF(pData) = pData;
	OOOF(uSize) = uSize;
	OOOF(iCacheData) = iCacheData;
}
OOOConstructorEnd
#undef OOOClass

