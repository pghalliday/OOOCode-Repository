#include "OOOUnitTestDefines.h"
#include "OOOCacheRepository.h"

static char * szMyData = "This is a test";
static char * szYourData = "This is also a test";

#define OOOClass Data
OOODeclare(char * szName, char * szError, unsigned char * pData, size_t uSize)
	OOOImplements
		OOOImplement(OOOIRepositoryData)
	OOOImplementsEnd
	OOOExports
		OOOExport(bool, checked)
	OOOExportsEnd
OOODeclareEnd

OOOPrivateData
	char * szName;
	char * szError;
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
	if (OOOF(szError))
	{
		if (OOOCheck(iError != NULL))
		{
			OOOCheck(O_strcmp(OOOICall(iError, toString), OOOF(szError)) == 0);
		}
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

OOOConstructor(char * szName, char * szError, unsigned char * pData, size_t uSize)
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
	OOOF(szError) = szError;
	OOOF(pData) = pData;
	OOOF(uSize) = uSize;
OOOConstructorEnd
#undef OOOClass

OOOTest(OOOCacheRepository)
{
	OOOIRepository * iRepository;
	OOOICache * iCache;
	Data * pData;
	char * szName;

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
	pData = OOOConstruct(Data, "MyData", "UNKNOWN MODULE", NULL, 0);
	OOOICall(iRepository, get, OOOCast(OOOIRepositoryData, pData));
	OOOCheck(OOOCall(pData, checked));
	OOODestroy(pData);

	/* Should return the same pointer for data that has been added to the cache */
	pData = OOOConstruct(Data, "MyData", NULL, (unsigned char *) szMyData, O_strlen(szMyData) + 1);
	OOOICall(iCache, set, "MyData", (unsigned char *) szMyData, O_strlen(szMyData) + 1);
	OOOICall(iRepository, get, OOOCast(OOOIRepositoryData, pData));
	OOOCheck(OOOCall(pData, checked));
	OOODestroy(pData);

	/* Should be able to add multiple modules to the cache */
	OOOICall(iCache, set, "YourData", (unsigned char *) szYourData, O_strlen(szYourData) + 1);
	pData = OOOConstruct(Data, "MyData", NULL, (unsigned char *) szMyData, O_strlen(szMyData) + 1);
	OOOICall(iRepository, get, OOOCast(OOOIRepositoryData, pData));
	OOOCheck(OOOCall(pData, checked));
	OOODestroy(pData);
	pData = OOOConstruct(Data, "YourData", NULL, (unsigned char *) szYourData, O_strlen(szYourData) + 1);
	OOOICall(iRepository, get, OOOCast(OOOIRepositoryData, pData));
	OOOCheck(OOOCall(pData, checked));
	OOODestroy(pData);
	pData = OOOConstruct(Data, "UnknownData", "UNKNOWN MODULE", NULL, 0);
	OOOICall(iRepository, get, OOOCast(OOOIRepositoryData, pData));
	OOOCheck(OOOCall(pData, checked));
	OOODestroy(pData);

	/* Should not keep a pointer to the original module name string */
	szName = O_strdup("Test");
	pData = OOOConstruct(Data, "Test", NULL, (unsigned char *) szMyData, O_strlen(szMyData) + 1);
	OOOICall(iCache, set, szName, (unsigned char *) szMyData, O_strlen(szMyData) + 1);
	/* ensure the memory is changed before freeing it */
	szName[0] = '\0';
	O_free(szName);
	OOOICall(iRepository, get, OOOCast(OOOIRepositoryData, pData));
	OOOCheck(OOOCall(pData, checked));
	OOODestroy(pData);

	OOODestroy(pRepository);
}
