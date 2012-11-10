#include "OOOUnitTestDefines.h"
#include "OOOCacheRepository.h"

OOOTest(OOOCacheRepository)
{
	OOOIRepository * iRepository;
	OOOICache * iCache;

	/* Should construct */
	OOOCacheRepository * pRepository = OOOConstruct(OOOCacheRepository);
	OOOCheck(pRepository != NULL);

	/* Should implement the OOOIRepository interface */
	iRepository = OOOCast(OOOIRepository, pRepository);
	OOOCheck(iRepository != NULL);

	/* Should implement the OOOICache interface */
	iCache = OOOCast(OOOICache, pRepository);
	OOOCheck(iCache != NULL);

	OOODestroy(pRepository);
}
