#include<tc/tc_startup.h>
#include<tcinit\tcinit.h>
#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<tc\emh.h>
#include<tccore\item.h>

int ITK_user_main(int argc, char* argv[])
{
	int iFail = 0;
	char *cError = NULL;

	iFail = ITK_auto_login();
	if (iFail == ITK_ok)
	{
		printf("Login Sucess");

	}
	else {

		EMH_ask_error_text(iFail, &cError);
		printf("The error is :%s\n", cError);
	}
	return iFail;
}