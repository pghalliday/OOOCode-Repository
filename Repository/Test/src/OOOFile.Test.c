#include "OOOUnitTestDefines.h"
#include "OOOFile.h"

OOOTest(OOOFile)
{
	OOOFile * pOOOFile = OOOConstruct(OOOFile);

	/* Check stuff here */
	OOOCheck(pOOOFile != NULL);

	OOODestroy(pOOOFile);
}
