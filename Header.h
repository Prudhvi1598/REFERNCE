#include<stdio.h>
#include<tcinit/tcinit.h>
#include<tc/tc_startup.h>
#include<bom/bom.h>
#include "Header.h"

#define LOGIN ITK_init_module("izn","izn","dba")

int ITK_user_main(int argc, char** argv)
{
	int iFail = 0;
	iFail = LOGIN;
	if (iFail == 0)
	{
		printf("TC Login successful!\n---------------------\n\n");

	}
	else
	{
		printf("TC Login Failed!\n-------------------------\n\n");
	}
	return ITK_ok;
}
















