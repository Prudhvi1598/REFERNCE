#include<iostream>
#include<tc/tc_startup.h>
#include<tcinit\tcinit.h>
#include<tc/emh.h>
#include<tccore/item.h>
#include<tccore/aom.h>
#include<tccore/aom_prop.h>
#include<tccore/workspaceobject.h>
using namespace std;
int check(int exp);
int ITK_user_main(int argc, char* argv[])
{
	int n = 0;
	tag_t *tList = NULL;
	char* cValue = NULL;
	check(ITK_init_module("hello", "hello", "dba"));
	cout << "Login sucessful\n" << endl;
	check(WSOM_find2("Fnd0Icon", &n, &tList));
	if (tList != NULL && n != 0)
	{
		for (int i =0; i < n; i++)
		{
			check(AOM_UIF_ask_value(tList[i], "fnd0IconId", &cValue));
			if (cValue != NULL)
			{
				cout << cValue << endl;
			}
		}
	}
	return 0;
	ITK_exit_module(true);
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
