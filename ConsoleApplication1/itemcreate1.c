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
	char *cError = NULL;
	tag_t tItem = NULLTAG;
	tag_t tRevision = NULLTAG;
	iFail = ITK_init_module("infodba", "infodba", "dba");
	if (iFail == ITK_ok)
	{
		printf("Login Sucessfully\n");
		iFail = ITEM_create_item("003331", "ITK3", "Item", "A", &tItem, &tRevision);
		if (iFail == ITK_ok)
		{
			printf("Item created Successfully\n");
			iFail = ITEM_save_item(tItem);
			if (iFail == ITK_ok)
			{
				printf("Saved successfully\n");
			}
			else {

				EMH_ask_error_text(iFail, &cError);
				printf("The error is :%s\n", cError);
			}
		}
		else {

			EMH_ask_error_text(iFail, &cError);
			printf("The error is :%s\n", cError);
		}
	}
	else {

		EMH_ask_error_text(iFail, &cError);
		printf("The error is :%s\n", cError);
	}
	return 0;
}