#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<tc\emh.h>
#include<tc/tc_startup.h>
#include<tcinit\tcinit.h>
#include<tccore\aom.h>
#include<tc\folder.h>
#include<sa\sa.h>
#include<sa\user.h>

int check(exp) 
{
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
	int pos =1;
	int instance = 1;
	tag_t tFolder = NULLTAG;
	tag_t tUser = NULLTAG;
	tag_t tTag = NULLTAG;
	tag_t tHomeFolder = NULLTAG;
	char *folderName = ITK_ask_cli_argument("-Folder=");
	char *user = ITK_ask_cli_argument("-u=");
	char *pass = ITK_ask_cli_argument("-p=");
	char *group = ITK_ask_cli_argument("-g=");

	if (user != NULL && pass != NULL && group != NULL)
	{
		ITK_init_module(user, pass, group);
		printf("Login Sucessfull\n");
		check(SA_find_user2(user, &tTag));
		if (tTag != NULLTAG)
		{
			printf("user tag find\n");
			check(SA_ask_user_home_folder(tTag, &tHomeFolder));
			if (tHomeFolder != NULLTAG)
			{
				printf("User home folder found\n");
				check(FL_create2(folderName, "", &tFolder));
				if (tFolder != NULLTAG)
				{
					printf("Folder created\n");
					AOM_save_without_extensions(tFolder);
					check(FL_insert(tHomeFolder, tFolder, pos));
					printf("Home folder added at given position\n");
					AOM_save_without_extensions(tHomeFolder);
					
				}
				else
				{
					printf("folder tag not found\n");
				}
			}
			else
			{
				printf("Home folder tag not found\n");
			}
		}
		else
		{
			printf("user tag not found\n");
		}

	}
	else
	{
		printf("Enter all credentials\n");
	}
	return 0;
}