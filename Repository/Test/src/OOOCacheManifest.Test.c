#include "OOOUnitTestDefines.h"
#include "OOOCacheManifest.h"

OOOTest(OOOCacheManifest)
{
	OOOCacheManifest * pCacheManifest = OOOConstruct(OOOCacheManifest);

	/* Check stuff here */
	OOOCheck(pCacheManifest != NULL);

	OOODestroy(pCacheManifest);
}
