#include<iostream>
#include<tc/tc_startup.h>
#include<tcinit\tcinit.h>
#include<tc/emh.h>
#include<tccore/item.h>
#include<tc/tc_macros.h>
#include<fstream>
#include<vector>
#include<string.h>
#include<string>
#include<sstream>
#include<algorithm>
int ITK_user_main(int argc, char* argv[])
{

	tag_t tItemTag = NULLTAG;
	char *cUser = ITK_ask_cli_argument("-u=");
	char *cPass = ITK_ask_cli_argument("-p=");
	char *cGroup = ITK_ask_cli_argument("-g=");
	const char *cFilePath = ITK_ask_cli_argument("-filepath=");
	if (cUser != NULL && cPass != NULL && cGroup != NULL)
	{
		ITKCALL(ITK_init_module(cUser, cPass, cGroup));
		cout << "Login sucessful\n" << endl;

	}
}