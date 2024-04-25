#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<tc/tc_startup.h>
#include<tcinit/tcinit.h>
#include<tc/emh.h>


int  ITK_user_main(int argc, char** argv[]) 
{
	    /* Variables*/
	char* eText ="";
	int iFail = ITK_ok;
		
	iFail = ITK_init_module("infodba","infodba","dba");
	if (iFail == ITK_ok)
		printf("\nLogin Successful");
	else
	 {
		EMH_ask_error_text(iFail, &eText);
		printf("\nError: %s", eText);
	 }
	return 0;
}