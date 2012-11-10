#include "OOOCacheRepository.h"

#define OOOClass OOOCacheRepository
OOOPrivateData
OOOPrivateDataEnd

OOODestructor
{
}
OOODestructorEnd

OOOMethod(void, set, char * szName, unsigned char * pData, size_t uSize)
{

}
OOOMethodEnd

OOOMethod(void, get, OOOIRepositoryData * iRepositoryData)
{

}
OOOMethodEnd

OOOConstructor()
{
#define OOOInterface OOOICache
	OOOMapVirtuals
		OOOMapVirtual(set)
	OOOMapVirtualsEnd
#undef OOOInterface

#define OOOInterface OOOIRepository
	OOOMapVirtuals
		OOOMapVirtual(get)
	OOOMapVirtualsEnd
#undef OOOInterface

	OOOMapMethods
	OOOMapMethodsEnd
}
OOOConstructorEnd
#undef OOOClass
