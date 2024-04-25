#include<tc/tc_startup.h>
#include<tcinit\tcinit.h>
#include<conio.h>
#include<stdio.h>
//#include<stdlib.h>
//#include<tccore\item.h>
//#include<tccore\aom_prop.h>
#define DATASET_H
#include<ae/ae_types.h>
#include<tccore/workspaceobject.h>
#include<common/tc_deprecation_macros.h>
#include<ae/libae_exports.h>

int ITK_user_main(int argc, char* argv[])
{
	int iFail = 0;
	char *error = NULL;
	tag_t tItem = NULLTAG;
	iFail = ITK_init_module("hello", "hello", "dba");
	if (iFail == ITK_ok)
	{
		printf("Login sucess\n");
		iFail=ae_create
		
	}
	else {

		EMH_ask_error_text(iFail, &error);
		printf("The error is :%s\n", error);
	}
	return iFail;
}