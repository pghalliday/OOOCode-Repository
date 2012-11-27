#include "OOOUnitTestDefines.h"
#include "OOOFile.h"
#include "OOODirectory.h"

#define TEST_DIRECTORY	"/test"
#define TEST_FILE	"/test/test"

#define OOOClass TestFileData
OOODeclarePrivate(char * szError, unsigned char * pData, size_t uSize)
	OOOImplements
		OOOImplement(OOOIFileReadData)
		OOOImplement(OOOIFileWriteData)
	OOOImplementsEnd
	OOOExports
		OOOExport(bool, wasChecked)
	OOOExportsEnd
OOODeclareEnd

OOOPrivateData
	char * szError;
	unsigned char * pData;
	size_t uSize;
	bool bChecked;
OOOPrivateDataEnd

OOODestructor
OOODestructorEnd

OOOMethod(void, data, OOOIError * iError, unsigned char * pData, size_t uSize)
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
		OOOCheck(iError == NULL);
	}
	OOOF(bChecked) = TRUE;
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

OOOConstructorPrivate(char * szError, unsigned char * pData, size_t uSize)
#define OOOInterface OOOIFileReadData
	OOOMapVirtuals
		OOOMapVirtual(data)
	OOOMapVirtualsEnd
#undef OOOInterface

#define OOOInterface OOOIFileWriteData
	OOOMapVirtuals
		OOOMapVirtual(getData)
		OOOMapVirtual(getSize)
		OOOMapVirtual(written)
	OOOMapVirtualsEnd
#undef OOOInterface

	OOOMapMethods
		OOOMapMethod(wasChecked)
	OOOMapMethodsEnd

	OOOF(szError) = szError;
	OOOF(pData) = pData;
	OOOF(uSize) = uSize;
OOOConstructorEnd
#undef OOOClass

static void read(OOODirectory * pDirectory, char * szPath, char * szError, unsigned char * pData, size_t uSize)
{
	OOOFile * pFile = OOOConstruct(OOOFile, pDirectory, szPath);
	TestFileData * pTestFileData = OOOConstruct(TestFileData, szError, pData, uSize);
	OOOICall(OOOCast(OOOIFile, pFile), read, OOOCast(OOOIFileReadData, pTestFileData));
	OOOCheck(OOOCall(pTestFileData, wasChecked));
	OOODestroy(pTestFileData);
	OOODestroy(pFile);
}

static void write(OOODirectory * pDirectory, char * szPath, char * szError, unsigned char * pData, size_t uSize)
{
	OOOFile * pFile = OOOConstruct(OOOFile, pDirectory, szPath);
	TestFileData * pTestFileData = OOOConstruct(TestFileData, szError, pData, uSize);
	OOOICall(OOOCast(OOOIFile, pFile), write, OOOCast(OOOIFileWriteData, pTestFileData));
	OOOCheck(OOOCall(pTestFileData, wasChecked));
	OOODestroy(pTestFileData);
	OOODestroy(pFile);
}

OOOTest(OOOFile)
{
	OOODirectory * pDirectory = OOOConstruct(OOODirectory, NULL, TEST_DIRECTORY);

	/* First delete the test directory to ensure that no old test files pollute our results */
	OOOCall(pDirectory, delete);

	/* Read should error if there is no such file */
	read(NULL, TEST_FILE, "No such file", NULL, 0);

	/* Write should error if the data can not be written */
	write(NULL, TEST_FILE, "Write failure", NULL, 0);

	OOODestroy(pDirectory);
}
