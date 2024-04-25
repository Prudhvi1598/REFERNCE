#include<stdio.h>
#include<tc/tc_startup.h>
#include<tcinit/tcinit.h>
#include<ae/dataset.h>

#define LOGIN ITK_init_module("izn","izn","dba");

int ITK_user_main(int argc, char* argv[])
{
	int iFail = 0;
	tag_t tdataset = NULLTAG;
	iFail = LOGIN;
	if (iFail == 0)
	{
		printf("Login Successfull!\n\n");
		AE_find_dataset2("hi", &tdataset);
		if (tdataset!=NULLTAG)
		{
			printf("Dataset Found!\n\n");
			iFail=AE_purge_dataset_revs(tdataset);
			if (iFail == 0)
			{
				printf("Purge Dataset versions Successfull!\n\n");
				AE_save_myself(tdataset);
			}
			else
			{
				printf("Purge Dataset versions Unsuccessfull!");
			}
		}
		else
		{
			printf("Dataset Found Failed!\n\n");
		}
	}
	else
	{
		printf("Login Failed!");
	}
	return ITK_ok;
}