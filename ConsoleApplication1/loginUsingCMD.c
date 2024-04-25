#include<tc/tc_startup.h>
#include<tcinit\tcinit.h>
#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<tc\emh.h>

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
	int iFail = 0;
	
	char *cUsername = ITK_ask_cli_argument("-u=");
	char *cPassword = ITK_ask_cli_argument("-p=");
	char *cGroup = ITK_ask_cli_argument("-g=");
	if (cUsername != NULL && cPassword != NULL && cGroup != NULL)
	{
		check(ITK_init_module(cUsername, cPassword, cGroup));
		printf("Login Sucess\n");
	}
	else
	{
		printf("Enter all credentials\n");
	}
	return iFail;
}