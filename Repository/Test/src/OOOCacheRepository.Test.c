#include "OOOUnitTestDefines.h"
#include "OOOCacheRepository.h"
#include "OOOFileSystem.h"

#define MY_DATA_NAME		"MyData"
#define MY_DATA				"This is a test"
#define YOUR_DATA_NAME		"YourData"
#define YOUR_DATA			"This is also a test"
#define UNKNOWN_DATA_NAME	"UnknownData"
#define TEMP_DATA_NAME		"TempData"

#define CACHE_DIRECTORY		"/cache"

void recursiveDelete(char * szPath)
{
	o_dir * pDir = O_dir_open(szPath);
	if (pDir)
	{
		o_dirent tDirent;
		char * szChildPath = O_calloc(O_strlen(szPath) + 1 + OTV_MAX_FILE_NAME_LENGTH + 1, sizeof(char));
		char * szInsert = szChildPath + O_strlen(szPath) + 1;
		O_strcpy(szChildPath, szPath);
		O_strcat(szChildPath, "/");
		while (O_dir_read(pDir, &tDirent))
		{
			o_stat tStat;
			O_strcpy(szInsert, tDirent.d_name);
			if (O_file_get_stat(szChildPath, &tStat) == SUCCESS)
			{
				if (tStat.permissions & OTV_DIRECTORY_FLAG == OTV_DIRECTORY_FLAG)
				{
					recursiveDelete(szChildPath);
				}
				else
				{
					assert(O_file_remove(szChildPath) == SUCCESS);
				}
			}
		}
		O_free(szChildPath);
		O_dir_close(pDir);
		assert(O_dir_remove(szPath) == SUCCESS);
	}
}

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

#define OOOClass TestFileData
OOODeclarePrivate(char * szError, char * szPath, unsigned char * pData, size_t uSize)
	OOOImplements
		OOOImplement(OOOIFileReadData)
		OOOImplement(OOOIFileWriteData)
		OOOImplement(OOOIFileRemoveData)
	OOOImplementsEnd
	OOOExports
		OOOExport(bool, wasChecked)
	OOOExportsEnd
OOODeclareEnd

OOOPrivateData
	char * szError;
	char * szPath;
	unsigned char * pData;
	size_t uSize;
	bool bChecked;
OOOPrivateDataEnd

OOODestructor
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
	if (OOOF(szError))
	{
		if (OOOCheck(iError != NULL))
		{
			OOOCheck(O_strcmp(OOOICall(iError, toString), OOOF(szError)) == 0);
		}
	}
	else
	{
		if (!OOOCheck(iError == NULL))
		{
			O_debug("Error: %s\n", OOOICall(iError, toString));
		}

		/* TODO: check the contents of the written file */
	}
	OOOF(bChecked) = TRUE;
}
OOOMethodEnd

OOOMethod(void, read, OOOIError * iError, unsigned char * pData, size_t uSize)
{
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
		if (!OOOCheck(iError == NULL))
		{
			O_debug("Error: %s\n", OOOICall(iError, toString));
		}
	}
	OOOF(bChecked) = TRUE;
}
OOOMethodEnd

OOOMethod(void, removed, OOOIError * iError)
{
	if (OOOF(szError))
	{
		if (OOOCheck(iError != NULL))
		{
			OOOCheck(O_strcmp(OOOICall(iError, toString), OOOF(szError)) == 0);
		}
	}
	else
	{
		if (!OOOCheck(iError == NULL))
		{
			O_debug("Error: %s\n", OOOICall(iError, toString));
		}

		/* TODO: check that the file was removed */
	}
	OOOF(bChecked) = TRUE;
}
OOOMethodEnd

OOOMethod(bool, wasChecked)
{
	return OOOF(bChecked);
}
OOOMethodEnd

OOOConstructorPrivate(char * szError, char * szPath, unsigned char * pData, size_t uSize)
{
#define OOOInterface OOOIFileWriteData
	OOOMapVirtuals
		OOOMapVirtual(getPath)
		OOOMapVirtual(getData)
		OOOMapVirtual(getSize)
		OOOMapVirtual(written)
	OOOMapVirtualsEnd
#undef OOOInterface

#define OOOInterface OOOIFileReadData
	OOOMapVirtuals
		OOOMapVirtual(getPath)
		OOOMapVirtual(read)
	OOOMapVirtualsEnd
#undef OOOInterface

#define OOOInterface OOOIFileRemoveData
	OOOMapVirtuals
		OOOMapVirtual(getPath)
		OOOMapVirtual(removed)
	OOOMapVirtualsEnd
#undef OOOInterface

	OOOMapMethods
		OOOMapMethod(wasChecked)
	OOOMapMethodsEnd

	OOOF(szError) = szError;
	OOOF(szPath) = szPath;
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

static void readFile(OOOFileSystem * pFileSystem, char * szPath, char * szError, unsigned char * pData, size_t uSize)
{
	TestFileData * pTestFileData = OOOConstruct(TestFileData, szError, szPath, pData, uSize);
	OOOICall(OOOCast(OOOIFileSystem, pFileSystem), readFile, OOOCast(OOOIFileReadData, pTestFileData));
	OOOCheck(OOOCall(pTestFileData, wasChecked));
	OOODestroy(pTestFileData);
}

OOOTest(OOOCacheRepository)
{
	OOOIRepository * iRepository;
	OOOICache * iCache;
	char * szName;

	OOOFileSystem * pFileSystem = OOOConstruct(OOOFileSystem);
	OOOCacheRepository * pRepository = OOOConstruct(OOOCacheRepository, OOOCast(OOOIFileSystem, pFileSystem), CACHE_DIRECTORY);

	/* start by removing the cache directory */
	recursiveDelete(CACHE_DIRECTORY);

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
//	readFile(pFileSystem, CACHE_DIRECTORY "/" MY_DATA_NAME, NULL, (unsigned char *) MY_DATA, O_strlen(MY_DATA) + 1);
//	readFile(pFileSystem, CACHE_DIRECTORY "/" YOUR_DATA_NAME, NULL, (unsigned char *) YOUR_DATA, O_strlen(YOUR_DATA) + 1);
//	readFile(pFileSystem, CACHE_DIRECTORY "/" TEMP_DATA_NAME, NULL, (unsigned char *) MY_DATA, O_strlen(MY_DATA) + 1);

	/* TODO: Should report errors encountered while writing modules to the file system in the directory specified */

	OOODestroy(pRepository);
	OOODestroy(pFileSystem);
}
