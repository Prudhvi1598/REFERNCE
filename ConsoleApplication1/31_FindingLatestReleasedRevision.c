#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<tc\emh.h>
#include<tc/tc_startup.h>
#include<tcinit\tcinit.h>
#include<tccore\aom_prop.h>
#include<tccore\item.h>
#define RELEASE_STATUS "release_status_list"
#define OBJECT_STRING "object_string"
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
	int i;
	int iNum;
	tag_t *tPValues = NULL;
	char *cRevName = NULL;
	char *cUser = ITK_ask_cli_argument("-u=");
	char *cPass = ITK_ask_cli_argument("-p=");
	char *cGroup = ITK_ask_cli_argument("-g=");
	const char *cSearchId= ITK_ask_cli_argument("-searchid=");

	if (cUser != NULL && cPass != NULL && cGroup != NULL)
	{
		check(ITK_init_module(cUser, cPass, cGroup));
		printf("Login successfull\n");
		check(ITEM_find_item(cSearchId, &tItemTag));
		if (tItemTag != NULLTAG)
		{
			printf("Item Tag found\n");
			check(ITEM_list_all_revs(tItemTag, &iCount, &tRevTags));
			if (tRevTags!=NULL&&iCount!=0)
			{
				printf("Revision tags listed\n");
				for (i = iCount-1; i > -1; i--)
				{
					check(AOM_ask_value_tags(tRevTags[i], RELEASE_STATUS, &iNum,&tPValues));//Here status are stored in the form of tags.
					if (tPValues != NULL)
					{
						printf("Values found\n");
						check(AOM_ask_value_string(tRevTags[i], OBJECT_STRING, &cRevName));
						if (cRevName != NULL)
						{
							printf("the latest released revision is %s\n", cRevName);
							break;
						}
						else
						{
							printf("Revision name not found\n");
						}
					}
					else
					{
						printf("Item not released\n");
					}
				}

				MEM_free(cRevName);
				MEM_free(tPValues);	
			}
			
			else
			{
				printf("Revision tags list not found\n");
			}
			MEM_free(tRevTags);
		}
		else
		{
			printf("Item tag not found\n");
		}
	}
	else
	{
		printf("Enter all credentials\n");
	}
	return 0;
	check(ITK_exit_module(true));
}
