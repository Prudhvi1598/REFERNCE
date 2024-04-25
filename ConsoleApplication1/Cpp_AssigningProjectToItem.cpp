#include<iostream>
#include<tc/tc_startup.h>
#include<tcinit\tcinit.h>
#include<tc/emh.h>
#include<tccore/item.h>
#include<tccore/project.h>
#include<tccore/aom.h>
using namespace std;
int check(int exp);
int ITK_user_main(int argc, char* argv[])
{
	
	int iFail = 0;
	char *error = NULL;
	tag_t tItem = NULLTAG;
	int iPcount = 1;
	int iOcount = 1;
	tag_t tProjectTag = NULLTAG;
	char *cUser = ITK_ask_cli_argument("-u=");
	char *cPass = ITK_ask_cli_argument("-p=");
	char *cGroup = ITK_ask_cli_argument("-g=");
	char *cItemId = ITK_ask_cli_argument("-id=");
	char *cProjId = ITK_ask_cli_argument("-pid=");
	if (cUser != NULL && cPass != NULL && cGroup != NULL)
	{
		check(ITK_init_module(cUser, cPass, cGroup));
		cout << "Login sucessful\n"<<endl;
		check(ITEM_find_item(cItemId, &tItem));
		if (tItem != NULLTAG)
		{
			//tag_t *tItem1 = tItem;
			cout << "Item tag found"<<endl;
			check(PROJ_find(cProjId, &tProjectTag));
			if (tProjectTag != NULLTAG)
			{
				
				check(PROJ_assign_objects(iPcount, &tProjectTag, iOcount,&tItem));
				cout<<"updated\n"<<endl;

			}
		}

	}

	return 0;
}
int check(int exp) {
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