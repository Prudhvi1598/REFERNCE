#include<stdio.h> 
#include<stdlib.h> 
#include<string.h> 
// teamcenter includes 
#include<tc/tc_startup.h> 
#include<tcinit\tcinit.h> 
#include<tc\emh.h> 
int ITK_user_main(int argc, char *argv[])
{
	int iFail = 0;
	char *cError = NULL;
	iFail = ITK_exit_module(true);
	if (iFail == ITK_ok)

	{
		printf("Logout Successful\n");

	}

	else {
		EMH_ask_error_text(iFail, &cError);
		printf("The error is :%s\n", cError);
	}

	return iFail;

}