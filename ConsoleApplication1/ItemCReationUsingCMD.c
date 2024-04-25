#include<tc/tc_startup.h>
#include<tcinit\tcinit.h>
#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<tc\emh.h>
#include<tccore\item.h>

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
	tag_t tRevision = NULLTAG;

	char *user = ITK_ask_cli_argument("-u=");
	char *pass = ITK_ask_cli_argument("-p=");
	char *group = ITK_ask_cli_argument("-g=");

	char *itemid = ITK_ask_cli_argument("-iid=");
	char *itemName = ITK_ask_cli_argument("-iname=");
	char *itemType = ITK_ask_cli_argument("-itype=");
	char *revId = ITK_ask_cli_argument("-irevid=");


	check(ITK_init_module(user,pass,group));
	printf("LoginSucessfully\n");
	if (itemid != NULL && itemName != NULL && itemType != NULL && revId != NULL)
	{
		check(ITEM_create_item(itemid, itemName, itemType, revId, &tItem, &tRevision));
		printf("item creation sucess\n");
		ITEM_save_item(tItem);
		printf("Item saved sucessfull\n");

	}
	else
	{
		printf("Enter all values(itemid,itemName,itemType,revId\n)");
	}
	return 0;
}