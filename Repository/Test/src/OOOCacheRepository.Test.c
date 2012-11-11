#include "OOOUnitTestDefines.h"
#include "OOOCacheRepository.h"

#define OOOClass Data
OOODeclare(char * szName, OOOIError * iError, unsigned char * pData, size_t uSize)
	OOOImplements
		OOOImplement(OOOIRepositoryData)
	OOOImplementsEnd
	OOOExports
		OOOExport(bool, checked)
	OOOExportsEnd
OOODeclareEnd

OOOPrivateData
	char * szName;
	OOOIError * iError;
	unsigned char * pData;
	size_t uSize;
	bool bChecked;
OOOPrivateDataEnd

OOODestructor
OOODestructorEnd

OOOMethod(char *, getName)
	return OOOF(szName);
OOOMethodEnd

OOOMethod(void, data, OOOIError * iError, unsigned char * pData, size_t uSize)
	OOOCheck(pData == OOOF(pData));
	OOOCheck(uSize == OOOF(uSize));
	if (OOOF(iError))
	{
		OOOCheck(O_strcmp(/* TODO */));
	}
	else
	{
		OOOCheck(iError == NULL);
	}
	OOOF(bChecked) = TRUE;
OOOMethodEnd

OOOMethod(bool, checked)
	return OOOF(bChecked);
OOOMethodEnd

OOOConstructor(char * szName, OOOIError * iError, unsigned char * pData, size_t uSize)
#define OOOInterface OOOIRepositoryData
	OOOMapVirtuals
		OOOMapVirtual(getName)
		OOOMapVirtual(data)
	OOOMapVirtualsEnd
#undef OOOInterface

	OOOMapMethods
		OOOMapMethod(checked)
	OOOMapMethodsEnd

	OOOF(szName) = szName;
	OOOF(iError) = iError;
	OOOF(pData) = pData;
	OOOF(uSize) = uSize;
OOOConstructorEnd
#undef OOOClass

OOOTest(OOOCacheRepository)
{
	OOOIRepository * iRepository;
	OOOICache * iCache;

	/* Should construct */
	OOOCacheRepository * pRepository = OOOConstruct(OOOCacheRepository);
	OOOCheck(pRepository != NULL);

	/* Should implement the OOOIRepository interface */
	iRepository = OOOCast(OOOIRepository, pRepository);
	OOOCheck(iRepository != NULL);

	/* Should implement the OOOICache interface */
	iCache = OOOCast(OOOICache, pRepository);
	OOOCheck(iCache != NULL);

	/* Should error when trying to get data that has not been added to the cache */


	OOODestroy(pRepository);
}
