/*
login to teamcenter
find user tag
Find folder
find child objects
iterate loop to print child objects
open file
save it in file
close file


*/
#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<tc\emh.h>
#include<tc/tc_startup.h>
#include<tcinit\tcinit.h>
#include<tccore\aom.h>
#include<tc\folder.h>
#include<tccore\workspaceobject.h>
#include<pom\pom\pom.h>
#include<tccore\aom_prop.h>
#define OBJECT_NAME "object_name"
int check(exp)
{
	int iFail = 0;
	char *cError = NULL;
	if (exp != iFail)
	{
		EMH_ask_error_text(iFail, &cError);
		printf("The error is :%s\n", cError);
		exit(0);
	}
	return 0;
}
int ITK_user_main(int argc, char* argv[])
{
	int iHits = 0;
	int i;
	char *class_name = NULL;
	char *cValues = NULL;
	
	tag_t *tList = NULL;
	tag_t *tClassId = NULLTAG;
	FL_sort_criteria_t sortCriteria = NULLTAG;
	int iNumOfReferences = 0;
	tag_t *tListOfReferences = NULL;
	FILE *fp=NULL;
	char *cUser = ITK_ask_cli_argument("-u=");
	char *cPass = ITK_ask_cli_argument("-p=");
	char *cGroup = ITK_ask_cli_argument("-g=");
	//char *cFilePath = ITK_ask_cli_argument("-filepath=");
	//const char *cMode= ITK_ask_cli_argument("-mode=");
	char *cWorkspaceObject = ITK_ask_cli_argument("-object=");
	if (cUser != NULL && cPass != NULL && cGroup)
	{
		check(ITK_init_module(cUser, cPass, cGroup));
		printf("Login Sucessfull\n");
		fp = fopen("D:\\filepath.txt", "w");
		printf("file opened\n");
		check(WSOM_find2(cWorkspaceObject, &iHits, &tList));
		if (iHits != 0)
		{
			for (i = 0; i < iHits; i++)
			{
				check(FL_ask_references(tList[i], sortCriteria, &iNumOfReferences, &tListOfReferences));
				if (iNumOfReferences != 0)
				{
					for (i = 0; i < iNumOfReferences; i++)
					{
						check(AOM_ask_value_string(tListOfReferences[i], OBJECT_NAME, &cValues));
						if (cValues!=NULL)
						{
							fprintf(fp, "%s\n", cValues);
							printf("%s\n", cValues);
							MEM_free(cValues);

						}
						else
						{
							printf("Value not found\n");
						}
					}
					MEM_free(tListOfReferences);
				}
				else
				{
					printf("NO references found\n");
				}
			}

		}
		else
		{
			printf("No hits found\n");
		}
	}
	fclose(fp);
	return 0;

	check(ITK_exit_module(true));
	
}
