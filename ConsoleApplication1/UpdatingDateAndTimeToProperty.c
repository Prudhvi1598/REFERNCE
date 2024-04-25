#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<tc\emh.h>
#include<tc/tc_startup.h>
#include<tcinit\tcinit.h>
#include<tccore\aom_prop.h>
#include<tccore/aom.h>
#include<tccore\item.h>
#include<time.h>
#include<dos.h>
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
	date_t value;
	tag_t tItemTag = NULLTAG;
	char *cUser = ITK_ask_cli_argument("-u=");
	char *cPass = ITK_ask_cli_argument("-p=");
	char *cGroup = ITK_ask_cli_argument("-g=");
	const char *cSearchId = ITK_ask_cli_argument("-searchid=");
	if (cUser != NULL && cPass != NULL && cGroup != NULL)
	{
		check(ITK_init_module(cUser, cPass, cGroup));
		printf("Login successfull\n");
		POM_get_latest_date(&value);
		printf("Date fetched\n");
		check(ITEM_find_item("000261",&tItemTag));
		if (tItemTag != NULLTAG)
		{
			printf("Item Tag find\n");
			AOM_refresh(tItemTag, true);
			check(AOM_set_value_date(tItemTag, "a4DateProp", value));
			printf("DAte value updated\n");
			AOM_save_without_extensions(tItemTag);
			printf("Saved\n");
			AOM_refresh(tItemTag, false);
		}
		
	}
	return 0;
}
