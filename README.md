OOOCode-Repository
==================

OOOCode Repository interface for retrieving named lumps of data.

## OOOIRepository interface

- Should support getting named data
- Should support asynchronous implementations

### Roadmap

- Nothing yet

## OOOCacheRepository

- Should implement the OOOICache interface

### Roadmap

- Should support chaining to other OOOCacheRepository instances
- Should cache data to the file system

## Roadmap

- Should implement a proxy repository to chain repositories that do not chain
- Should implement an in band repository to load data from broadcast
- Should implement a remote repository to load data from the network

## API

### To implement an OOOIRepository

MyRepository.h

```C
#ifndef OOOMyRepository_H
#define OOOMyRepository_H

#include "OOOIRepository.h"

#define OOOClass OOOMyRepository
OOODeclare()
    OOOImplements
        OOOImplement(OOOIRepository)
    OOOImplementsEnd
    OOOExports
    OOOExportsEnd
OOODeclareEnd
#undef OOOClass

#endif
```

MyRepository.c

```C
#include "OOOMyRepository.h"

#define OOOClass OOOMyRepository

OOOPrivateData
OOOPrivateDataEnd

OOODestructor
{
}
OOODestructorEnd

OOOMethod(void, get, OOOIRepositoryData * iRepositoryData)
{
    unsigned char * pData;
    size_t uSize;
    char * szName = OOOICall(iRepositoryData, getName);

    /* TODO: Retrieve the data */

    /* Notify the cache data instance that caching is complete */
    OOOICall(iRepositoryData, data, NULL, pData, uSize);
}
OOOMethodEnd

OOOConstructor()
{
#define OOOInterface OOOIRepository
    OOOMapVirtuals
        OOOMapVirtual(get)
    OOOMapVirtualsEnd
#undef OOOInterface
}
OOOConstructorEnd

#undef OOOClass
```

### To use OOOCacheRepository

```C
#include "OOOCacheRepository.h"

OOOTestRepository * pRepository;
unsigned char pMyData[] =
{
    ...
}
size_t uMySize = sizeof(pMyData);

/* Declare a private data class */

#define OOOClass MyPrivateData
OOODeclare(char * szName, unsigned char * pData, size_t uSize)
    OOOImplements
        OOOImplement(OOOICacheData)
        OOOImplement(OOOIRepositoryData)
    OOOImplementsEnd
    OOOExports
    OOOExportsEnd
OOODeclareEnd
#undef

static void start()
{
    MyPrivateData * pMyPrivateData = OOOConstruct(MyPrivateData, "Test", pMyData, uMySize);
    pRepository = OOOConstruct(OOOCacheRepository);
    OOOICall(OOOCast(OOOICache, pRepository), set, OOOCast(OOOICacheData, pMyPrivateData));
}

static void cached(MyPrivateData * pMyPrivateData, OOOIError * iError)
{
    assert(iError == NULL);
    OOOICall(OOOCast(OOOIRepository, pRepository), get, OOOCast(OOOIRepositoryData, pMyPrivateData));
}

static void data(MyPrivateData * pMyPrivateData, OOOIError * iError, unsigned char * pData, size_t uSize)
{
    OOODestroy(pMyPrivateData);

    assert(iError == NULL);
    assert(pData == pMyData);
    assert(uSize == uMySize);

    OOODestroy(pRepository);
}


/* Implement the private data class */

#define OOOClass MyPrivateData
OOOPrivateData
    char * szName;
    unsigned char * pData;
    size_t uSize;
OOOPrivateDataEnd

OOODestructor
OOODestructorEnd

OOOMethod(char *, getName)
    return OOOF(szName);
OOOMethodEnd

OOOMethod(unsigned char *, getData)
    return OOOF(pData);
OOOMethodEnd

OOOMethod(size_t, getSize)
    return OOOF(uSize);
OOOMethodEnd

OOOMethod(void, cached, OOOIError * iError)
    cached(OOOThis, iError);
OOOMethodEnd

OOOMethod(void, data, OOOIError * iError, unsigned char * pData, size_t uSize)
    data(OOOThis, iError, pData, uSize);
OOOMethodEnd

OOOConstructor(char * szName, unsigned char * pData, size_t uSize)
#define OOOInterface OOOICacheData
    OOOMapVirtuals
        OOOMapVirtual(getName)
        OOOMapVirtual(getData)
        OOOMapVirtual(getSize)
        OOOMapVirtual(cached)
    OOOMapVirtualsEnd
#undef OOOInterface

#define OOOInterface OOOIRepositoryData
    OOOMapVirtuals
        OOOMapVirtual(getName)
        OOOMapVirtual(data)
    OOOMapVirtualsEnd
#undef OOOInterface

    OOOMapMethods
    OOOMapMethodsEnd

    OOOF(szName) = szName;
    OOOF(pData) = pData;
    OOOF(uSize) = uSize;
OOOConstructorEnd
#undef OOOClass
```

## Contributing

In lieu of a formal styleguide, take care to maintain the existing coding style. Add unit tests for any new or changed functionality.

## Prerequisites

- OpenTV SDK C2.2 from http://community.opentv.com

## Initialising

This project references sub-modules so after cloning the repository...

```
git submodule init
git submodule update
```

Then the easiest way to work with the project in the OpenTV IDE...

1. Create a new workspace in the root of the project
1. In the IDE choose **File/Import...**
1. In the resulting dialog choose **General/Existing Projects Into Workspace** and press **Next**
1. Choose the **Select Root Directory:** Radio button and **Browse...** to the root of the project
1. A number of projects may be listed under **Projects**
1. Select the projects in the root, deselect those under the submodules folder and then click **Finish**
1. You should then be able to build the OpenTV projects by choosing **Project/Build All**

After you have built the projects at least once run the test application in the Virtual Set top box by...

1. Choosing the **RepositoryFlow** project on the left
1. Pressing the green **Run** button at the top
1. In the resulting dialog choose **Local OpenTV Application** and press **OK**

## Release History
_(Nothing yet)_

## License
Copyright (c) 2012 Peter Halliday  
Licensed under the MIT license.