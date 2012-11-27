#ifndef OOOFile_H
#define OOOFile_H

#include "OOOCode.h"
#include "OOODirectory.h"
#include "OOOIFile.h"

#define OOOClass OOOFile
OOODeclare(OOODirectory * pParentDirectory, char * szPath)
	OOOImplements
		OOOImplement(OOOIFile)
	OOOImplementsEnd
	OOOExports
	OOOExportsEnd
OOODeclareEnd
#undef OOOClass

#endif
