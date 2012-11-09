#include "OOOInMemoryRepository.h"
#include "OOOError.h"

#define OOOClass OOOInMemoryRepository

typedef struct _Entry Entry;
struct _Entry
{
	char * szName;
	unsigned char * pData;
	size_t uSize;
	Entry * pNext;
};

OOOPrivateData
	OOOICache * iCache;
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

OOOMethod(void, get, OOOIRepositoryData * iRepositoryData)
{
	Entry * pEntry = OOOF(pEntries);
	char * szName = OOOICall(iRepositoryData, getName);
	while (pEntry)
	{
		if (O_strcmp(pEntry->szName, szName) == 0)
		{
			OOOICall(OOOF(iCache), set, szName, pEntry->pData, pEntry->uSize);
			OOOICall(iRepositoryData, load, NULL, pEntry->pData, pEntry->uSize);
			break;
		}
		pEntry = pEntry->pNext;
	}

	if (!pEntry)
	{
		OOOError * pError = OOOConstruct(OOOError, "UNKNOWN MODULE");
		OOOICall(iRepositoryData, load, OOOCast(OOOIError, pError), NULL, 0);
		OOODestroy(pError);
	}
}
OOOMethodEnd

OOOMethod(void, add, char * szName, unsigned char * pData, size_t uSize)
{
	Entry * pEntry = O_malloc(sizeof(Entry));
	pEntry->pNext = OOOF(pEntries);
	OOOF(pEntries) = pEntry;
	pEntry->pData = pData;
	pEntry->uSize = uSize;
	pEntry->szName = O_strdup(szName);
}
OOOMethodEnd

OOOConstructor(OOOICache * iCache)
{
	#define OOOInterface OOOIRepository
	OOOMapVirtuals
		OOOMapVirtual(get)
	OOOMapVirtualsEnd
	#undef OOOInterface

	OOOMapMethods
		OOOMapMethod(add)
	OOOMapMethodsEnd

	OOOF(iCache) = iCache;
}
OOOConstructorEnd

#undef OOOClass
