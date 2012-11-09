#include "OOOUnitTestDefines.h"
#include "OOOInMemoryRepository.h"
#include "OOOTestCache.h"

static char * szApple = "Apple";
static char * szBanana = "Banana";
static char * szPear = "Pear";

#define OOOClass TestRepositoryData

OOODeclare()
	OOOImplements
		OOOImplement(OOOIRepositoryData)
	OOOImplementsEnd
	OOOExports
		OOOExport(void, start)
	OOOExportsEnd
OOODeclareEnd

OOOPrivateData
	OOOTestCache * pCache;
	OOOInMemoryRepository * pRepository;
	char * szName;
	bool bApple;
	bool bBanana;
	bool bPear;
OOOPrivateDataEnd

OOODestructor
{
	OOODestroy(OOOF(pRepository));
	OOODestroy(OOOF(pCache));
}
OOODestructorEnd

OOOMethod(void, start)
{
	OOOF(szName) = "APPLE";
	OOOICall(OOOCast(OOOIRepository, OOOF(pRepository)), get, OOOCast(OOOIRepositoryData, OOOThis));
}
OOOMethodEnd

OOOMethod(char *, getName)
{
	return OOOF(szName);
}
OOOMethodEnd

OOOMethod(void, load, OOOIError * iError, unsigned char * pData, size_t uSize)
{
	unsigned char * pCachedData;
	size_t uCachedSize;

	if (O_strcmp(OOOF(szName), "APPLE") == 0 && !OOOF(bApple))
	{
		OOOCheck(O_strcmp(OOOICall(iError, toString), "UNKNOWN MODULE") == 0);
		OOOCheck(pData == NULL);
		OOOCheck(0 == uSize);
		OOOF(szName) = "BANANA";
		OOOICall(OOOCast(OOOIRepository, OOOF(pRepository)), get, OOOCast(OOOIRepositoryData, OOOThis));
	}
	else if (O_strcmp(OOOF(szName), "BANANA") == 0 && !OOOF(bBanana))
	{
		OOOCheck(O_strcmp(OOOICall(iError, toString), "UNKNOWN MODULE") == 0);
		OOOCheck(pData == NULL);
		OOOCheck(0 == uSize);
		OOOCall(OOOF(pRepository), add, "APPLE", (unsigned char *) szApple, O_strlen(szApple) + 1);
		OOOF(bApple) = TRUE;
		OOOF(szName) = "APPLE";
		OOOICall(OOOCast(OOOIRepository, OOOF(pRepository)), get, OOOCast(OOOIRepositoryData, OOOThis));
	}
	else if (O_strcmp(OOOF(szName), "APPLE") == 0 && OOOF(bApple))
	{
		OOOCheck(iError == NULL);
		OOOCheck(O_strcmp((char *) pData, szApple) == 0);
		OOOCheck(O_strlen(szApple) + 1 == uSize);

		/* cache should contain APPLE which should reference the original pointer */
		OOOCall(OOOF(pCache), get, "APPLE", &pCachedData, &uCachedSize);
		OOOCheck(pCachedData == (unsigned char *) szApple);
		OOOCheck(uCachedSize == O_strlen(szApple) + 1);

		OOOF(szName) = "PEAR";
		OOOICall(OOOCast(OOOIRepository, OOOF(pRepository)), get, OOOCast(OOOIRepositoryData, OOOThis));
	}
	else if (O_strcmp(OOOF(szName), "PEAR") == 0 && !OOOF(bPear))
	{
		OOOCheck(O_strcmp(OOOICall(iError, toString), "UNKNOWN MODULE") == 0);
		OOOCheck(pData == NULL);
		OOOCheck(0 == uSize);
		OOOCall(OOOF(pRepository), add, "BANANA", (unsigned char *) szBanana, O_strlen(szBanana) + 1);
		OOOF(bBanana) = TRUE;
		OOOF(szName) = "BANANA";
		OOOICall(OOOCast(OOOIRepository, OOOF(pRepository)), get, OOOCast(OOOIRepositoryData, OOOThis));
	}
	else if (O_strcmp(OOOF(szName), "BANANA") == 0 && OOOF(bBanana))
	{
		OOOCheck(iError == NULL);
		OOOCheck(O_strcmp((char *) pData, szBanana) == 0);
		OOOCheck(O_strlen(szBanana) + 1 == uSize);

		/* cache should contain BANANA which should reference the original pointer */
		OOOCall(OOOF(pCache), get, "BANANA", &pCachedData, &uCachedSize);
		OOOCheck(pCachedData == (unsigned char *) szBanana);
		OOOCheck(uCachedSize == O_strlen(szBanana) + 1);

		OOOF(szName) = "PEAR";
		OOOCall(OOOF(pRepository), add, "PEAR", (unsigned char *) szPear, O_strlen(szPear) + 1);
		OOOF(bPear) = TRUE;
		OOOF(szName) = "PEAR";
		OOOICall(OOOCast(OOOIRepository, OOOF(pRepository)), get, OOOCast(OOOIRepositoryData, OOOThis));
	}
	else if (O_strcmp(OOOF(szName), "PEAR") == 0 && OOOF(bPear))
	{
		OOOCheck(iError == NULL);
		OOOCheck(O_strcmp((char *) pData, szPear) == 0);
		OOOCheck(O_strlen(szPear) + 1 == uSize);

		/* cache should contain PEAR which should reference the original pointer */
		OOOCall(OOOF(pCache), get, "PEAR", &pCachedData, &uCachedSize);
		OOOCheck(pCachedData == (unsigned char *) szPear);
		OOOCheck(uCachedSize == O_strlen(szPear) + 1);
	}
	else
	{
		OOOCheck(FALSE);
	}
}
OOOMethodEnd

OOOConstructor()
{
	#define OOOInterface OOOIRepositoryData
	OOOMapVirtuals
		OOOMapVirtual(getName)
		OOOMapVirtual(load)
	OOOMapVirtualsEnd
	#undef OOOInterface

	OOOMapMethods
		OOOMapMethod(start)
	OOOMapMethodsEnd

	OOOF(pCache) = OOOConstruct(OOOTestCache);
	OOOF(pRepository) = OOOConstruct(OOOInMemoryRepository, OOOCast(OOOICache, OOOF(pCache)));
}
OOOConstructorEnd
#undef OOOClass


OOOTest(OOOInMemoryRepository)
{
	TestRepositoryData * pTestRepositoryData = OOOConstruct(TestRepositoryData);
	OOOCall(pTestRepositoryData, start);
	OOODestroy(pTestRepositoryData);
}
