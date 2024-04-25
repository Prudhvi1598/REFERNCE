#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<tc\emh.h>
#include<tc/tc_startup.h>
#include<tcinit\tcinit.h>
#include<tccore\aom_prop.h>
#include<tccore\item.h>
#include<tccore/aom.h>
#define RELEASE_STATUS "release_status_list"
#define OBJECT_DESC "object_desc"
//#define 
int check(exp) {
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
	tag_t tItemTag = NULLTAG;
	int iCount = 0;
	tag_t *tRevTags = NULL;
	tag_t sec_obj_t = NULLTAG;
	int i;
	int iNum;
	tag_t *tPValues = NULL;
	char *cUser = ITK_ask_cli_argument("-u=");
	char *cPass = ITK_ask_cli_argument("-p=");
	char *cGroup = ITK_ask_cli_argument("-g=");
	//const char *cSearchId = ITK_ask_cli_argument("-searchid=");
	char *cSearchId = NULL;
	char *cDesc = NULL;
	FILE *fp;
	char cBuff[40];
	char* cFileContent;
	
	
	//if (cUser != NULL && cPass != NULL && cGroup != NULL)
	//{
		check(ITK_init_module(cUser, cPass, cGroup));
		printf("Login successfull\n");

		fp = fopen("C:\\Users\\T50315\\Desktop\\item.txt", "r");
		while (fgets(cBuff, 40, fp) != NULL)      //Reading file line by line
		{
			cFileContent = strtok(cBuff, "\n"); //Removing new line character from a line
			cSearchId = strtok(cFileContent, ",");     //Tokenizing
			printf("%s\n", cSearchId);

			cDesc = strtok(NULL, ",");
			printf("%s\n", cDesc);
		}
		check(ITEM_find_item(cSearchId, &tItemTag));
		if (tItemTag != NULLTAG)
		{
			printf("Item Tag found\n");
			check(ITEM_list_all_revs(tItemTag, &iCount, &tRevTags));
			if (tRevTags != NULL)
			{
				printf("Revision tags listed\n");
				if (iCount >= 3)
				{
					check(AOM_ask_value_tags(tRevTags[iCount - 1], RELEASE_STATUS, &iNum, &tPValues));
					if (tPValues != NULL)
					{
						Fun(tRevTags[iCount - 1], cDesc);
					
					}
					else
					{
						for (i = iCount - 2; i >= iCount - 3; i--)
						{

							Fun(tRevTags[i], cDesc);

						}
					}
				}
				else if (iCount < 3 && iCount>1)
				{
					check(AOM_ask_value_tags(tRevTags[iCount - 1], RELEASE_STATUS, &iNum, &tPValues));
					if (tPValues != NULL)
					{
						Fun(tRevTags[iCount - 1], cDesc);
					}
					else
					{
						for (i = iCount - 2; i >= 0; i--)
						{
							Fun(tRevTags[i], cDesc);
						}
					}
				}
				else if (iCount == 1)
				{
					check(AOM_ask_value_tags(tRevTags[iCount - 1], RELEASE_STATUS, &iNum, &tPValues));
					if (tPValues != NULL)
					{
						Fun(tRevTags[iCount - 1], cDesc);
					}
					else
					{
						printf("There is only 1 item revision that is also not released\n");
					}
				}
			}
		//}
		MEM_free(tRevTags);
		MEM_free(tPValues);
	}
	return 0;
}
int Fun(tag_t obj,char *cDesc)
{
	check(ITK_set_bypass(true));
	check(AOM_set_value_string(obj, OBJECT_DESC, cDesc));
	printf("Desc updated for same item rev\n");
	check(AOM_save_without_extensions(obj));
	printf("Item revision saved\n");
	check(ITK_set_bypass(false));
	return 0;
}