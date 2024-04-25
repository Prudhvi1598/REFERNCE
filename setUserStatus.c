#include<stdio.h>
#include<tcinit/tcinit.h>
#include<tc/tc_startup.h>
#include<tccore/aom.h>
#include<sa/user.h>


#define u "infodba"
#define p "infodba"
#define g "dba"

int ITK_user_main(int argc, char** argv)
{
	int iFail = 0;
	int istatus = 0;
	tag_t tuser = NULLTAG;
	iFail = ITK_init_module(u, p, g);
	if (iFail == ITK_ok)
	{
		printf("TC Login Successful!\n\n");
		SA_find_user2("dummy", &tuser);
		if (tuser != 0)
		{
			SA_get_user_status(tuser, &istatus);
			printf("The status of User is %d\n\n", istatus);
			if (istatus == 0)
			{
				AOM_lock(tuser);
				SA_set_user_status(tuser, 1);
				AOM_save_without_extensions(tuser);
				printf("The status of User is set to inactive\n\n");
			}
			else
			{
				AOM_lock(tuser);
				SA_set_user_status(tuser, 0);
				AOM_save_without_extensions(tuser);
				printf("The status of User is set to active\n\n");
			}
		}
		else
		{
			printf("\n\nUser Not Found\n\n");
		}
	}
	else
	{
		printf("TC Login Unsuccessful");
	}

	return ITK_ok;
}