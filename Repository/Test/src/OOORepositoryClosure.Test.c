#include "OOOUnitTestDefines.h"
#include "OOOError.h"
#include "OOOIRepositoryData.h"

#define TEST_CLOSURE_DATA	"This is a test"
#define TEST_NAME			"This is a test name"
#define TEST_ERROR			"This is a test error"
#define TEST_DATA			"This is test data"

#define OOOClass TestTarget
OOODeclarePrivate()
	OOOImplements
	OOOImplementsEnd
	OOOExports
		OOOExport(void, close, OOOIRepositoryData * iRepositoryData)
	OOOExportsEnd
OOODeclareEnd

OOOPrivateData
OOOPrivateDataEnd

OOODestructor
OOODestructorEnd

OOOMethod(void, close, OOOIRepositoryData * iRepositoryData)
{
	char * szName = OOOICall(iRepositoryData, getName);
	OOOError * pError = OOOConstruct(OOOError, TEST_ERROR);
	if (O_strcmp(szName, TEST_NAME) != 0)
	{
		OOOError("Expected: %s: Received: %s", TEST_NAME, szName);
	}
	OOOICall(iRepositoryData, data, OOOCast(OOOIError, pError), (unsigned char *) TEST_DATA, O_strlen(TEST_DATA) + 1);
	OOODestroy(pError);
}
OOOMethodEnd

OOOConstructorPrivate()
{
	OOOMapMethods
		OOOMapMethod(close)
	OOOMapMethodsEnd
}
OOOConstructorEnd
#undef OOOClass

#define OOOClass Test
OOODeclarePrivate()
	OOOImplements
	OOOImplementsEnd
	OOOExports
		OOOExport(void, start)
	OOOExportsEnd
OOODeclareEnd

// declare the test closure class
#define OOOClosure TestClosure
#define OOOClosureType char *
#include "OOORepositoryClosure.h"
#undef OOOClosureType
#undef OOOClosure

OOOPrivateData
	TestTarget * pTestTarget;
	bool bChecked;
OOOPrivateDataEnd

OOODestructor
{
	OOODestroy(OOOF(pTestTarget));
}
OOODestructorEnd

OOOMethod(void, data, char * szClosureData, OOOIError * iError, unsigned char * pData, size_t uSize)
{
	char * szError = OOOICall(iError, toString);
	if (O_strcmp(szClosureData, TEST_CLOSURE_DATA) != 0)
	{
		OOOError("Expected: %s: Received: %s", TEST_CLOSURE_DATA, szClosureData);
	}
	if (O_strcmp(szError, TEST_ERROR) != 0)
	{
		OOOError("Expected: %s: Received: %s", TEST_ERROR, szError);
	}
	if (O_strcmp((char *) pData, TEST_DATA) != 0)
	{
		OOOError("Expected: %s: Received: %s", TEST_DATA, (char *) pData);
	}
	OOOCheck(uSize == O_strlen(TEST_DATA) + 1);
	OOOF(bChecked) = TRUE;
}
OOOMethodEnd

OOOMethod(void, start)
{
	TestClosure * pClosure = OOOClosureConstruct
	(
		TestClosure,
		data,
		TEST_CLOSURE_DATA,
		TEST_NAME
	);
	OOOCall(OOOF(pTestTarget), close, OOOCast(OOOIRepositoryData, pClosure));
	OOOCheck(OOOF(bChecked));
}
OOOMethodEnd

OOOConstructorPrivate()
{
	OOOMapMethods
		OOOMapMethod(start)
	OOOMapMethodsEnd

	OOOF(pTestTarget) = OOOConstruct(TestTarget);
}
OOOConstructorEnd
#undef OOOClass

OOOTest(OOORepositoryClosure)
{
	Test * pTest = OOOConstruct(Test);
	OOOCall(pTest, start);
	OOODestroy(pTest);
}
