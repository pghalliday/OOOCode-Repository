#include "OOOFile.h"
#include "OOOError.h"

#define OOOClass OOOFile

OOOPrivateData
OOOPrivateDataEnd

OOODestructor
{
}
OOODestructorEnd

OOOMethod(void, read, OOOIFileReadData * iFileReadData)
{
	OOOError * pError = OOOConstruct(OOOError, "No such file");
	OOOICall(iFileReadData, data, OOOCast(OOOIError, pError), NULL, 0);
	OOODestroy(pError);
}
OOOMethodEnd

OOOMethod(void, write, OOOIFileWriteData * iFileWriteData)
{
	OOOError * pError = OOOConstruct(OOOError, "Write failure");
	OOOICall(iFileWriteData, written, OOOCast(OOOIError, pError));
	OOODestroy(pError);
}
OOOMethodEnd

OOOConstructor(OOODirectory * pParentDirectory, char * szPath)
{
#define OOOInterface OOOIFile
	OOOMapVirtuals
		OOOMapVirtual(read)
		OOOMapVirtual(write)
	OOOMapVirtualsEnd
#undef OOOInterface

	OOOMapMethods
	OOOMapMethodsEnd
}
OOOConstructorEnd

#undef OOOClass
