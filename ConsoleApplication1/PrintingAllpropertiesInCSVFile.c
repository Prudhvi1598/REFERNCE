
#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<tc\emh.h>
#include<tccore\item.h>
#include<tc/tc_startup.h>
#include<tcinit\tcinit.h>
#include<tccore\aom_prop.h>
#include<tccore\aom.h>
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
	int iCount = 0;//o
	char **cPropNames = NULL;//of
	char *cValues = NULL;
	int i;

	tag_t tItem = NULLTAG;
	FILE *fp = NULL;
	char *cUser = ITK_ask_cli_argument("-u=");
	char *cPass = ITK_ask_cli_argument("-p=");
	char *cGroup = ITK_ask_cli_argument("-g=");
	char *cItemId= ITK_ask_cli_argument("-itemid=");
	if (cUser != NULL && cPass != NULL && cGroup)
	{
		check(ITK_init_module(cUser, cPass, cGroup));
		printf("login sucess\n");
		fp = fopen("D:\\ItemProp1.csv", "w+");
		printf("Fille opened\n");
		check(ITEM_find_item(cItemId, &tItem));
		if (tItem != NULLTAG)
		{
			check(AOM_ask_prop_names(tItem, &iCount, &cPropNames));
			if (cPropNames != NULL)
			{
				for (i = 0; i < iCount; i++)
				{
					check(AOM_UIF_ask_value(tItem, cPropNames[i], &cValues));
					if (cValues != NULL)
					{
						fprintf(fp, "%s %s\n", cPropNames[i], cValues);
						
					}
					else
					{
						printf("Value not found\n");
					}
				}
				MEM_free(cPropNames);
				MEM_free(cValues);
			}
			else
			{
				printf(" Property Names not found\n");
			}
		}
		else
		{
			printf("Item Tag not found\n");
		}

	}
	else
	{
		printf("Enter all credentials\n");
	}
	
		
	fclose(fp);
	return 0;
}