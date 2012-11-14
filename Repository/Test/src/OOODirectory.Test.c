#include "OOOUnitTestDefines.h"
#include "OOODirectory.h"

#define TEST_DIRECTORY1	"/test1"
#define TEST_DIRECTORY2	"test2"
#define TEST_DIRECTORY3	"test3"
#define TEST_DIRECTORY	TEST_DIRECTORY1 "/" TEST_DIRECTORY2 "/" TEST_DIRECTORY3

void recursiveDelete(char * szPath)
{
	o_dir * pDir = O_dir_open(szPath);
	if (pDir)
	{
		o_dirent tDirent;
		char * szChildPath = O_calloc(O_strlen(szPath) + 1 + OTV_MAX_FILE_NAME_LENGTH + 1, sizeof(char));
		char * szInsert = szChildPath + O_strlen(szPath) + 1;
		O_strcpy(szChildPath, szPath);
		O_strcat(szChildPath, "/");
		while (O_dir_read(pDir, &tDirent))
		{
			o_stat tStat;
			O_strcpy(szInsert, tDirent.d_name);
			if (O_file_get_stat(szChildPath, &tStat) == SUCCESS)
			{
				if (tStat.permissions & OTV_DIRECTORY_FLAG == OTV_DIRECTORY_FLAG)
				{
					recursiveDelete(szChildPath);
				}
				else
				{
					assert(O_file_remove(szChildPath) == SUCCESS);
				}
			}
		}
		O_free(szChildPath);
		O_dir_close(pDir);
		assert(O_dir_remove(szPath) == SUCCESS);
	}
}

bool isDirectory(char * szPath)
{
	bool bIsDirectory = FALSE;
	o_stat tStat;
	if (O_file_get_stat(szPath, &tStat) == SUCCESS)
	{
		if (tStat.permissions & OTV_DIRECTORY_FLAG == OTV_DIRECTORY_FLAG)
		{
			bIsDirectory = TRUE;
		}
	}
	return bIsDirectory;
}

char * resolvePath(char * szPath)
{
	char * szAbsolutePath;
	if (szPath[0] == '/')
	{
		szAbsolutePath = O_strdup(szPath);
	}
	else
	{
		char * szCurrentDirectory = O_dir_get_path(NULL, 0);
		szAbsolutePath = O_calloc(O_strlen(szCurrentDirectory) + 1 + O_strlen(szPath) + 1, sizeof(char));
		O_strcpy(szAbsolutePath, szCurrentDirectory);
		O_free(szCurrentDirectory);
		O_strcat(szAbsolutePath, "/");
		O_strcat(szAbsolutePath, szPath);
	}
	return szAbsolutePath;
}

OOOTest(OOODirectory)
{
	char * szAbsolutePath;
	OOODirectory * pDirectory1 = OOOConstruct(OOODirectory, NULL, TEST_DIRECTORY1);
	OOODirectory * pDirectory2 = OOOConstruct(OOODirectory, pDirectory1, TEST_DIRECTORY2);
	OOODirectory * pDirectory3 = OOOConstruct(OOODirectory, pDirectory2, TEST_DIRECTORY3);
	OOODirectory * pRelativeDirectory = OOOConstruct(OOODirectory, NULL, TEST_DIRECTORY3);

	/* start by removing the test directories */
	recursiveDelete(TEST_DIRECTORY1);

	/* Should be able to get the absolute path */
	OOOCheck(O_strcmp(TEST_DIRECTORY1, OOOCall(pDirectory1, getPath)) == 0);
	OOOCheck(O_strcmp(TEST_DIRECTORY, OOOCall(pDirectory3, getPath)) == 0);

	/* Relative paths with no parent specified should resolve relative to the current directory */
	szAbsolutePath = resolvePath(TEST_DIRECTORY3);
	OOOCheck(O_strcmp(szAbsolutePath, OOOCall(pRelativeDirectory, getPath)) == 0);
	O_free(szAbsolutePath);

	/* Should create a directory including parent directories */
	OOOCheck(OOOCall(pDirectory3, create));
	OOOCheck(isDirectory(TEST_DIRECTORY));

	/* Should recursively delete a directory */
	OOOCheck(OOOCall(pDirectory1, delete));
	OOOCheck(!isDirectory(TEST_DIRECTORY1));

	OOODestroy(pRelativeDirectory);
	OOODestroy(pDirectory3);
	OOODestroy(pDirectory2);
	OOODestroy(pDirectory1);
}
