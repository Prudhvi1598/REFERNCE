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
#include<lov\lov.h>
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
	char *cItem = NULL;
	tag_t tItemRev = NULLTAG;
	char *cPreference = "CustomPreference";
	check(ITK_init_module("hello", "hello", "dba"));//loging into the Teamcenter module
	printf("Login Sucessfull\n");
	int iCount = 0;
	int iCount1 = 2;
	char **cValues = NULL;
	check(PREF_ask_char_values(cPreference, &iCount, &cValues));
	for (int i = 0; i < iCount; i++)
	{
		printf("%s\n", cValues[i]);
		tag_t tItem = NULLTAG;
		check(ITEM_find_item("000230", &tItem));
		if (tItem!=NULLTAG)
		{
			AOM_refresh(tItem, true);
			check(AOM_set_value_string(tItem, "object_desc",cValues[i]));
		
			AOM_save_without_extensions(tItem);
			AOM_refresh(tItem, false);
			printf("Value updated\n");
		}
	}
	MEM_free(cValues);
	return 0;
}
