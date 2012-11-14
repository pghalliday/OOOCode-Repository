#include "OOODirectory.h"
#include "limits.h"

#define OOOClass OOODirectory

OOOPrivateData
	char * szPath;
OOOPrivateDataEnd

OOODestructor
{
	O_free(OOOF(szPath));
}
OOODestructorEnd

OOOMethod(char *, getPath)
{
	return OOOF(szPath);
}
OOOMethodEnd

OOOMethod(bool, create)
{
	bool bSuccess = TRUE;
	o_stat tStat;
	if (O_file_get_stat(OOOF(szPath), &tStat) == SUCCESS)
	{
		if (tStat.permissions & OTV_DIRECTORY_FLAG != OTV_DIRECTORY_FLAG)
		{
			bSuccess = FALSE;
		}
	}
	else
	{
		char * szLastPath = O_strrchr(OOOF(szPath), '/');
		char * szParentPath = O_calloc(szLastPath - OOOF(szPath) + 1, sizeof(char));
		O_strncpy(szParentPath, OOOF(szPath), szLastPath - OOOF(szPath));
		if (O_strlen(szParentPath) > 0)
		{
			OOODirectory * pDirectory = OOOConstruct(OOODirectory, NULL, szParentPath);
			bSuccess = OOOCall(pDirectory, create);
			OOODestroy(pDirectory);
		}
		O_free(szParentPath);
		if (bSuccess)
		{
			bSuccess = (O_dir_create(OOOF(szPath), OTV_WORLD_READ_WRITE_PERMISSION, UINT_MAX) == SUCCESS);
		}
	}
	return bSuccess;
}
OOOMethodEnd

OOOMethod(bool, delete)
{
	bool bSuccess = TRUE;
	o_dir * pDir = O_dir_open(OOOF(szPath));
	if (pDir)
	{
		o_dirent tDirent;
		char * szChildPath = O_calloc(O_strlen(OOOF(szPath)) + 1 + OTV_MAX_FILE_NAME_LENGTH + 1, sizeof(char));
		char * szInsert = szChildPath + O_strlen(OOOF(szPath)) + 1;
		O_strcpy(szChildPath, OOOF(szPath));
		O_strcat(szChildPath, "/");
		while (bSuccess && O_dir_read(pDir, &tDirent))
		{
			o_stat tStat;
			O_strcpy(szInsert, tDirent.d_name);
			if (O_file_get_stat(szChildPath, &tStat) == SUCCESS)
			{
				if (tStat.permissions & OTV_DIRECTORY_FLAG == OTV_DIRECTORY_FLAG)
				{
					OOODirectory * pDirectory = OOOConstruct(OOODirectory, NULL, szChildPath);
					bSuccess = OOOCall(pDirectory, delete);
					OOODestroy(pDirectory);
				}
				else
				{
					bSuccess = (O_file_remove(szChildPath) == SUCCESS);
				}
			}
		}
		O_free(szChildPath);
		O_dir_close(pDir);
		if (bSuccess)
		{
			bSuccess = (O_dir_remove(OOOF(szPath)) == SUCCESS);
		}
	}
	else
	{
		bSuccess = FALSE;
	}
	return bSuccess;
}
OOOMethodEnd

OOOConstructor(OOODirectory * pParent, char * szPath)
{
	assert(szPath);

	OOOMapMethods
		OOOMapMethod(getPath)
		OOOMapMethod(create)
		OOOMapMethod(delete)
	OOOMapMethodsEnd

	if (szPath[0] == '/')
	{
		/* szPath is an absolute path */
		OOOF(szPath) = O_strdup(szPath);
	}
	else
	{
		/* szPath is a relative path */
		if (pParent)
		{
			/* szPath is relative to the given parent directory */
			char * szParent = OOOCall(pParent, getPath);
			OOOF(szPath) = O_calloc(O_strlen(szParent) + 1 + O_strlen(szPath) + 1, sizeof(char));
			O_strcpy(OOOF(szPath), szParent);
		}
		else
		{
			/* szPath is relative to the current directory */
			char * szCurrentDirectory = O_dir_get_path(NULL, 0);
			OOOF(szPath) = O_calloc(O_strlen(szCurrentDirectory) + 1 + O_strlen(szPath) + 1, sizeof(char));
			O_strcpy(OOOF(szPath), szCurrentDirectory);
			O_free(szCurrentDirectory);
		}
		O_strcat(OOOF(szPath), "/");
		O_strcat(OOOF(szPath), szPath);
	}
}
OOOConstructorEnd

#undef OOOClass
