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
		O_free(pLast->pData);
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
			size_t uSize = pEntry->uSize;
			unsigned char * pData = O_calloc(uSize, sizeof(unsigned char));
			O_memcpy(pData, pEntry->pData, uSize);
			OOOICall(iRepositoryData, load, NULL, pData, uSize);
			O_free(pData);
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
	pEntry->pData = O_calloc(uSize, sizeof(unsigned char));
	O_memcpy(pEntry->pData, pData, uSize);
	pEntry->uSize = uSize;
	pEntry->szName = O_strdup(szName);
}
OOOMethodEnd

OOOConstructor()
{
	#define OOOInterface OOOIRepository
	OOOMapVirtuals
		OOOMapVirtual(get)
	OOOMapVirtualsEnd
	#undef OOOInterface

	OOOMapMethods
		OOOMapMethod(add)
	OOOMapMethodsEnd
}
OOOConstructorEnd

#undef OOOClass
