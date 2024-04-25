#include<tc/tc_startup.h>
#include<tcinit\tcinit.h>
#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<tc\emh.h>
#include<tccore\item.h>
#include<tccore\aom.h>

int ITK_user_main(int argc, char* argv[])
{
	int iFail = 0;
	char *error;
	const char *item_desc = "hi";
	tag_t tItem = NULLTAG;
	tag_t Revision = NULLTAG;

	iFail = ITK_init_module("hello", "hello", "dba");
	if (iFail == ITK_ok)
	{
		printf("Login sucessfull\n");
		iFail = ITEM_create_item("001110", "itk0", "Item", "A", &tItem, &Revision);
		if (iFail == ITK_ok)
		{
			printf("creation sucess\n");
			iFail = ITEM_set_description(tItem, item_desc);
			if (iFail == ITK_ok)
			{
				printf("description set sucess\n");
				iFail = ITEM_save_item(tItem);
			}
			else
			{
				EMH_ask_error_text(iFail, &error);
				printf("The error is :%s\n", error);
			}
		}
		else
		{
			EMH_ask_error_text(iFail, &error);
			printf("The error is :%s\n", error);
		}
	}
	else
	{
		EMH_ask_error_text(iFail, &error);
		printf("The error is :%s\n", error);
	}
	return iFail;
}
