#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<tc\emh.h>
#include<tccore\item.h>
#include<tccore\aom_prop.h>
#include<tccore\aom.h>
#include<tcinit\tcinit.h>
#include<tc/tc_startup.h>
#include<tc\preferences.h>
#define OBJECT_DESC "object_desc"
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
	
	tag_t tItem = NULLTAG;
	char *cPreference = "CustomFilePreference";
	char *cUser = ITK_ask_cli_argument("-u=");
	char *cPass = ITK_ask_cli_argument("-p=");
	char *cGroup = ITK_ask_cli_argument("-g=");
	check(ITK_init_module(cUser, cPass, cGroup));//loging into the Teamcenter module
	printf("Login Sucessfull\n");
	int iCount = 0;
	char **cValues = NULL;
	char *cPropValue = NULL;
	FILE *fp;
	char *cItem = ITK_ask_cli_argument("-itemid=");
	check(PREF_ask_char_values(cPreference, &iCount, &cValues));//finding the values of preference
	for (int i = 0; i < iCount; i++)
	{
		printf("%s", cValues[i]);
		tag_t tItem = NULLTAG;
		check(ITEM_find_item(cItem, &tItem)); //finding item
		if (tItem != NULLTAG)
		{
			printf("Item tag find\n");
			check(AOM_ask_value_string(tItem, OBJECT_DESC, &cPropValue));
			if (cPropValue != NULL)
			{
				printf("%s\n", cPropValue);
				fp = fopen(cValues[i], "w");
				fprintf(fp,"%s\n",cPropValue);
				MEM_free(cPropValue);
			}
		}
	}
	MEM_free(cValues);
	return 0;
}
