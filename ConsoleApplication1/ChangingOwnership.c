/*
CHANGING OWNERSHIP OF AN ITEM


*/


#include<tc/tc_startup.h>
#include<tcinit\tcinit.h>
#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<tc\emh.h>
#include<tccore\item.h>
#include<tccore\aom.h>
#include<sa\user.h>
#include<sa\group.h>
int ITK_user_main(int argc, char* argv[])
{
	int iFail = 0;
	char *cError=NULL;
	tag_t tItem = NULLTAG;
	const char *cUser = "hello";
	tag_t tUserTag = NULLTAG;
	tag_t tGroupTag = NULLTAG;
	char *user = ITK_ask_cli_argument("-u=");
	char *pass = ITK_ask_cli_argument("-p=");
	char *group = ITK_ask_cli_argument("-g=");

	iFail = ITK_init_module(user, pass, group);
	if (iFail == ITK_ok)
	{
		printf("LoginSucessfully\n");
		iFail = ITEM_find_item("000099", &tItem);
		if (iFail == ITK_ok)
		{
			printf("Iem Find\n");
			iFail = SA_find_user2(cUser, &tUserTag);
			if (iFail == ITK_ok)
			{
				printf("User tag find\n");
				iFail = SA_find_group("dba", &tGroupTag);
				if (iFail == ITK_ok)
					{
					printf("Group Tag find\n");
					iFail = AOM_set_ownership(tItem, tUserTag, tGroupTag);
					if (iFail == ITK_ok)
					{
						printf("Owner ship changed\n");
						AOM_save_without_extensions(tItem);
					}
					else
					{

						EMH_ask_error_text(iFail, &cError);
						printf("The error is :%s\n", cError);
					}
				}
				else
				{

					EMH_ask_error_text(iFail, &cError);
					printf("The error is :%s\n", cError);
				}
			}
			else
			{

				EMH_ask_error_text(iFail, &cError);
				printf("The error is :%s\n", cError);
			}
		}
		else
		{

			EMH_ask_error_text(iFail, &cError);
			printf("The error is :%s\n", cError);
		}
	}
	else
	{

		EMH_ask_error_text(iFail, &cError);
		printf("The error is :%s\n", cError);
	}
	return iFail;
}
