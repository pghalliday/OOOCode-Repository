#include "OOOUnitTestDefines.h"
#include "OOOCacheRepository.h"
#include "OOOFile.h"

#define MY_DATA_NAME		"MyData"
#define MY_DATA				"This is a test"
#define YOUR_DATA_NAME		"YourData"
#define YOUR_DATA			"This is also a test"
#define UNKNOWN_DATA_NAME	"UnknownData"
#define TEMP_DATA_NAME		"TempData"

#define CACHE_DIRECTORY		"/cache"

#define OOOClass TestCacheData
OOODeclare(char * szName, char * szError, unsigned char * pData, size_t uSize)
	OOOImplements
		OOOImplement(OOOICacheData)
	OOOImplementsEnd
	OOOExports
		OOOExport(bool, wasChecked)
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

OOOMethod(unsigned char *, getData)
	return OOOF(pData);
OOOMethodEnd

OOOMethod(size_t, getSize)
	return OOOF(uSize);
OOOMethodEnd

OOOMethod(void, cached, OOOIError * iError)
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

OOOMethod(bool, wasChecked)
	return OOOF(bChecked);
OOOMethodEnd

OOOConstructor(char * szName, char * szError, unsigned char * pData, size_t uSize)
#define OOOInterface OOOICacheData
	OOOMapVirtuals
		OOOMapVirtual(getName)
		OOOMapVirtual(getData)
		OOOMapVirtual(getSize)
		OOOMapVirtual(cached)
	OOOMapVirtualsEnd
#undef OOOInterface

	OOOMapMethods
		OOOMapMethod(wasChecked)
	OOOMapMethodsEnd

	OOOF(szName) = szName;
	OOOF(szError) = szError;
	OOOF(pData) = pData;
	OOOF(uSize) = uSize;
OOOConstructorEnd
#undef OOOClass

#define OOOClass TestRepositoryData
OOODeclare(char * szName, char * szError, unsigned char * pData, size_t uSize)
	OOOImplements
		OOOImplement(OOOIRepositoryData)
	OOOImplementsEnd
	OOOExports
		OOOExport(bool, wasChecked)
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

OOOMethod(bool, wasChecked)
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
		OOOMapMethod(wasChecked)
	OOOMapMethodsEnd

	OOOF(szName) = szName;
	OOOF(szError) = szError;
	OOOF(pData) = pData;
	OOOF(uSize) = uSize;
OOOConstructorEnd
#undef OOOClass

static void set(OOOICache * iCache, char * szName, char * szError, unsigned char * pData, size_t uSize)
{
	TestCacheData * pTestCacheData = OOOConstruct(TestCacheData, szName, szError, pData, uSize);
	OOOICall(iCache, set, OOOCast(OOOICacheData, pTestCacheData));
	OOOCheck(OOOCall(pTestCacheData, wasChecked));
	OOODestroy(pTestCacheData);
}

static void get(OOOIRepository * iRepository, char * szName, char * szError, unsigned char * pData, size_t uSize)
{
	TestRepositoryData * pTestRepositoryData = OOOConstruct(TestRepositoryData, szName, szError, pData, uSize);
	OOOICall(iRepository, get, OOOCast(OOOIRepositoryData, pTestRepositoryData));
	OOOCheck(OOOCall(pTestRepositoryData, wasChecked));
	OOODestroy(pTestRepositoryData);
}

OOOTest(OOOCacheRepository)
{
	OOOIRepository * iRepository;
	OOOICache * iCache;
	char * szName;

	OOODirectory * pDirectory = OOOConstruct(OOODirectory, NULL, CACHE_DIRECTORY);
	OOOCacheRepository * pRepository = OOOConstruct(OOOCacheRepository, pDirectory);

	/* start by removing the cache directory */
	OOOCall(pDirectory, delete);

	/* Should implement the OOOIRepository interface */
	iRepository = OOOCast(OOOIRepository, pRepository);
	OOOCheck(iRepository != NULL);

	/* Should implement the OOOICache interface */
	iCache = OOOCast(OOOICache, pRepository);
	OOOCheck(iCache != NULL);

	/* Should error when trying to get data that has not been added to the cache */
	get(iRepository, MY_DATA_NAME, "UNKNOWN MODULE", NULL, 0);

	/* Should return the same pointer for data that has been added to the cache */
	set(iCache, MY_DATA_NAME, NULL, (unsigned char *) MY_DATA, O_strlen(MY_DATA) + 1);
	get(iRepository, MY_DATA_NAME, NULL, (unsigned char *) MY_DATA, O_strlen(MY_DATA) + 1);

	/* Should be able to add multiple modules to the cache */
	set(iCache, YOUR_DATA_NAME, NULL, (unsigned char *) YOUR_DATA, O_strlen(YOUR_DATA) + 1);
	get(iRepository, MY_DATA_NAME, NULL, (unsigned char *) MY_DATA, O_strlen(MY_DATA) + 1);
	get(iRepository, YOUR_DATA_NAME, NULL, (unsigned char *) YOUR_DATA, O_strlen(YOUR_DATA) + 1);
	get(iRepository, UNKNOWN_DATA_NAME, "UNKNOWN MODULE", NULL, 0);

	/* Should not keep a pointer to the original module name string */
	szName = O_strdup(TEMP_DATA_NAME);
	set(iCache, szName, NULL, (unsigned char *) MY_DATA, O_strlen(MY_DATA) + 1);
	/* ensure the memory is changed before freeing it */
	szName[0] = '\0';
	O_free(szName);
	get(iRepository, TEMP_DATA_NAME, NULL, (unsigned char *) MY_DATA, O_strlen(MY_DATA) + 1);

	/* Should write modules to the file system in the directory specified */

	/* Should report errors encountered while writing modules to the file system in the directory specified */

	OOODestroy(pRepository);
	OOODestroy(pDirectory);
}
