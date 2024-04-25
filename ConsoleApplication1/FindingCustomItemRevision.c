#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<tc\emh.h>
#include<tccore\item.h>
#include<tc/tc_startup.h>
#include<tcinit\tcinit.h>
#include<tccore\aom_prop.h>
int ITK_user_main(int argc, char* argv[])
{

	int iFail = 0;
	char *cError = NULL;
	tag_t tRevision = NULLTAG;
	char *values = NULL;
	iFail = ITK_init_module("infodba", "infodba", "dba");
	if (iFail == ITK_ok)
	{
		printf("Login Sucessfully\n");
		iFail = ITEM_find_rev("000229","A",&tRevision);
		if (tRevision!=NULLTAG)
		{
			iFail = AOM_ask_value_string(tRevision, "object_name", &values);
			printf("item revision find %s", values);
		}
		else 
		{
			EMH_ask_error_text(iFail, &cError);
			printf("The error is :%s\n", cError);
		}
	}
	else {

		EMH_ask_error_text(iFail, &cError);
		printf("The error is :%s\n", cError);
	}
	return iFail;
}