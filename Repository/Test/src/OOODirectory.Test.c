#include "OOOUnitTestDefines.h"
#include "OOODirectory.h"

#define TEST_DIRECTORY	"/test"

OOOTest(OOODirectory)
{
	OOODirectory * pOOODirectory = OOOConstruct(OOODirectory, TEST_DIRECTORY);

	/* Check stuff here */
	OOOCheck(pOOODirectory != NULL);

	OOODestroy(pOOODirectory);
}
