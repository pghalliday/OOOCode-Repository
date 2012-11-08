#include "OOOUnitTestDefines.h"
#include "OOOInMemoryRepository.h"

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
	OOOInMemoryRepository * pRepository;
	char * szName;
	bool bApple;
	bool bBanana;
	bool bPear;
OOOPrivateDataEnd

OOODestructor
{
	OOODestroy(OOOF(pRepository));
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

	OOOF(pRepository) = OOOConstruct(OOOInMemoryRepository);
}
OOOConstructorEnd
#undef OOOClass


OOOTest(OOOInMemoryRepository)
{
	TestRepositoryData * pTestRepositoryData = OOOConstruct(TestRepositoryData);
	OOOCall(pTestRepositoryData, start);
	OOODestroy(pTestRepositoryData);
}
