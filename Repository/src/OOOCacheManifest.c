#include "OOOCacheManifest.h"

#define OOOClass OOOCacheManifest

OOOPrivateData
	unsigned char * pSerializedData;
	size_t uSerializedSize;
OOOPrivateDataEnd

OOODestructor
{
	if (OOOF(pSerializedData))
	{
		O_free(OOOF(pSerializedData));
	}
}
OOODestructorEnd

OOOMethod(void, deserialize, unsigned char * pData, size_t uSize)
{
	if (OOOF(pSerializedData))
	{
		O_free(OOOF(pSerializedData));
		OOOF(pSerializedData) = NULL;
		OOOF(uSerializedSize) = 0;
	}
	if (uSize)
	{
		/*
		 * TODO: deserialize pData
		 */
		OOOF(pSerializedData) = O_calloc(uSize, sizeof(unsigned char));
		O_memcpy(OOOF(pSerializedData), pData, uSize);
		OOOF(uSerializedSize) = uSize;
	}
}
OOOMethodEnd

OOOMethod(unsigned char *, getSerializedData)
{
	return OOOF(pSerializedData);
}
OOOMethodEnd

OOOMethod(size_t, getSerializedSize)
{
	return OOOF(uSerializedSize);
}
OOOMethodEnd

OOOMethod(size_t, getSize)
{
	/*
	 * TODO: calculate current cache size
	 */
	return 0;
}
OOOMethodEnd

OOOMethod(char *, getOldest)
{
	/* TODO */
	return NULL;
}
OOOMethodEnd

OOOMethod(void, add, char * szFile, size_t uSize)
{
	/* TODO */
}
OOOMethodEnd

OOOMethod(void, remove, char * szFile)
{
	/* TODO */
}
OOOMethodEnd

OOOConstructor()
{
	OOOMapMethods
		OOOMapMethod(deserialize)
		OOOMapMethod(getSerializedData)
		OOOMapMethod(getSerializedSize)
		OOOMapMethod(getSize)
		OOOMapMethod(getOldest)
		OOOMapMethod(add)
		OOOMapMethod(remove)
	OOOMapMethodsEnd
}
OOOConstructorEnd

#undef OOOClass
