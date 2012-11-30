#include "OOOUnitTestDefines.h"
#include "OOOCacheRepository.h"
#include "OOOFileSystem.h"

#define MY_DATA_NAME		"MyData"
#define MY_DATA				"123456789"	/* 10 bytes including the 0 terminator */
#define YOUR_DATA_NAME		"YourData"
#define YOUR_DATA			"987654321"	/* 10 bytes including the 0 terminator */
#define UNKNOWN_DATA_NAME	"UnknownData"
#define TEMP_DATA_NAME		"TempData"
#define EXCESS_DATA_NAME	"ExcessData"

#define CACHE_DIRECTORY		"/cache"
#define CACHE_SIZE			35

/*
 * Private ICacheData implementation
 */

#define OOOClass TestCacheData
OOODeclarePrivate(char * szName, char * szError, unsigned char * pData, size_t uSize)
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

OOOConstructorPrivate(char * szName, char * szError, unsigned char * pData, size_t uSize)
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

/*
 * Private OOOIRepositoryData implementation
 */

#define OOOClass TestRepositoryData
OOODeclarePrivate(char * szName, char * szError, unsigned char * pData, size_t uSize)
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

OOOConstructorPrivate(char * szName, char * szError, unsigned char * pData, size_t uSize)
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

/*
 * Private OOOIDirectoryRemoveData implementation
 */

#define OOOClass Directory
OOODeclarePrivate(char * szPath)
	OOOImplements
		OOOImplement(OOOIDirectoryRemoveData)
	OOOImplementsEnd
	OOOExports
	OOOExportsEnd
OOODeclareEnd

OOOPrivateData
	char * szPath;
OOOPrivateDataEnd

OOODestructor
OOODestructorEnd

OOOMethod(char *, getPath)
{
	return OOOF(szPath);
}
OOOMethodEnd

OOOMethod(void, removed, OOOIError * iError)
{
	if (iError)
	{
		OOOInfo("Error encountered deleting cache directory (probably doesn't exist): %s", OOOICall(iError, toString));
	}
}
OOOMethodEnd

OOOConstructorPrivate(char * szPath)
{
#define OOOInterface OOOIDirectoryRemoveData
	OOOMapVirtuals
		OOOMapVirtual(getPath)
		OOOMapVirtual(removed)
	OOOMapVirtualsEnd
#undef OOOInterface

	OOOMapMethods
	OOOMapMethodsEnd

	OOOF(szPath) = szPath;
}
OOOConstructorEnd
#undef OOOClass

/*
 * Private OOOIFileReadData implementation
 */

#define OOOClass File
OOODeclarePrivate(char * szPath, char * szError, unsigned char * pData, size_t uSize)
	OOOImplements
		OOOImplement(OOOIFileReadData)
	OOOImplementsEnd
	OOOExports
	OOOExportsEnd
OOODeclareEnd

OOOPrivateData
	char * szPath;
	char * szError;
	unsigned char * pData;
	size_t uSize;
OOOPrivateDataEnd

OOODestructor
OOODestructorEnd

OOOMethod(char *, getPath)
{
	return OOOF(szPath);
}
OOOMethodEnd

OOOMethod(void, read, OOOIError * iError, unsigned char * pData, size_t uSize)
{
	if (OOOF(szError))
	{
		if (OOOCheck(iError != NULL))
		{
			char * szError = OOOICall(iError, toString);
			if (O_strcmp(OOOF(szError), szError) != 0)
			{
				OOOError("expected: %s: received: %s", OOOF(szError), szError);
			}
		}
	}
	else
	{
		if (iError)
		{
			OOOError(OOOICall(iError, toString));
		}
		else
		{
			OOOCheck(uSize == OOOF(uSize));
			if (OOOF(pData))
			{
				if (OOOCheck((bool) pData))
				{
					OOOCheck(O_strcmp(pData, OOOF(pData)) == 0);
				}
			}
			else
			{
				OOOCheck(pData == NULL);
			}
		}
	}
}
OOOMethodEnd

OOOConstructorPrivate(char * szPath, char * szError, unsigned char * pData, size_t uSize)
{
#define OOOInterface OOOIFileReadData
	OOOMapVirtuals
		OOOMapVirtual(getPath)
		OOOMapVirtual(read)
	OOOMapVirtualsEnd
#undef OOOInterface

	OOOMapMethods
	OOOMapMethodsEnd

	OOOF(szPath) = szPath;
	OOOF(szError) = szError;
	OOOF(pData) = pData;
	OOOF(uSize) = uSize;
}
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

static void removeDirectory(OOOFileSystem * pFileSystem, char * szPath)
{
	Directory * pDirectory = OOOConstruct(Directory, szPath);
	OOOICall(OOOCast(OOOIFileSystem, pFileSystem), removeDirectory, OOOCast(OOOIDirectoryRemoveData, pDirectory));
	OOODestroy(pDirectory);
}

static void readFile(OOOFileSystem * pFileSystem, char * szPath, char * szError, unsigned char * pData, size_t uSize)
{
	File * pFile = OOOConstruct(File, szPath, szError, pData, uSize);
	OOOICall(OOOCast(OOOIFileSystem, pFileSystem), readFile, OOOCast(OOOIFileReadData, pFile));
	OOODestroy(pFile);
}

OOOTest(OOOCacheRepository)
{
	OOOIRepository * iRepository;
	OOOICache * iCache;
	char * szName;

	OOOFileSystem * pFileSystem = OOOConstruct(OOOFileSystem);
	OOOCacheRepository * pRepository = OOOConstruct(OOOCacheRepository, OOOCast(OOOIFileSystem, pFileSystem), CACHE_DIRECTORY, CACHE_SIZE);

	/* start by removing the cache directory */
	removeDirectory(pFileSystem, CACHE_DIRECTORY);

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
	readFile(pFileSystem, CACHE_DIRECTORY "/" MY_DATA_NAME, NULL, (unsigned char *) MY_DATA, O_strlen(MY_DATA) + 1);
	readFile(pFileSystem, CACHE_DIRECTORY "/" YOUR_DATA_NAME, NULL, (unsigned char *) YOUR_DATA, O_strlen(YOUR_DATA) + 1);
	readFile(pFileSystem, CACHE_DIRECTORY "/" TEMP_DATA_NAME, NULL, (unsigned char *) MY_DATA, O_strlen(MY_DATA) + 1);

	/*
	 * Should limit the file system usage - 3 x 10 byte files have already
	 * been written, writing another should push us over the 35 byte limit.
	 * This should result in the MY_DATA_NAME file being deleted as it was
	 * used least recently
	 */
//	set(iCache, EXCESS_DATA_NAME, NULL, (unsigned char *) YOUR_DATA, O_strlen(YOUR_DATA) + 1);
//	readFile(pFileSystem, CACHE_DIRECTORY "/" MY_DATA_NAME, "", NULL, 0);
//	readFile(pFileSystem, CACHE_DIRECTORY "/" YOUR_DATA_NAME, NULL, (unsigned char *) YOUR_DATA, O_strlen(YOUR_DATA) + 1);
//	readFile(pFileSystem, CACHE_DIRECTORY "/" TEMP_DATA_NAME, NULL, (unsigned char *) MY_DATA, O_strlen(MY_DATA) + 1);
//	readFile(pFileSystem, CACHE_DIRECTORY "/" EXCESS_DATA_NAME, NULL, (unsigned char *) YOUR_DATA, O_strlen(MY_DATA) + 1);

	/* TODO: should delete the cache directory on set if the manifest cannot be loaded */

	OOODestroy(pRepository);
	OOODestroy(pFileSystem);
}
