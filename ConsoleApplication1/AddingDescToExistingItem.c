#include<tc/tc_startup.h>
#include<tcinit\tcinit.h>
#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<tc\emh.h>
#include<tccore\item.h>
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
	tag_t tItem = NULLTAG;
	char *user = ITK_ask_cli_argument("-u=");
	char *pass = ITK_ask_cli_argument("-p=");
	char *group = ITK_ask_cli_argument("-g=");
	const char *item_id= ITK_ask_cli_argument("-iid=");
	const char *item_desc = ITK_ask_cli_argument("-desc=");
	check(ITK_init_module(user, pass, group));
	printf("Login Sucess\n");
	check(ITEM_find_item(item_id,&tItem));
	if (tItem != NULLTAG)
	{
		printf("Item tag found\n");
		check(ITEM_set_description(tItem, item_desc));
		printf("Item description updated\n");
		ITEM_save_item(tItem);
		printf("Item saved\n");
	}
	return 0;
}