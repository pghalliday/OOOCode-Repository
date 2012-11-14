#ifndef OOODirectory_H
#define OOODirectory_H

#include "OOOCode.h"
#include "OOOIError.h"

#define OOOClass OOODirectory
OOODeclare(OOODirectory * pParent, char * szPath)
	OOOImplements
	OOOImplementsEnd
	OOOExports
		OOOExport(char *, getPath)
		OOOExport(bool, create)
		OOOExport(bool, delete)
	OOOExportsEnd
OOODeclareEnd
#undef OOOClass

#endif
