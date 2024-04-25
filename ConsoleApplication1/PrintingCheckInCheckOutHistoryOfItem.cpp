/*
-----checkin and checkout history of an item------

input format -u=<userid> -p=<password> -g=<group> -itemid=<itemid>


----out put format-----

Check in and check out history of Item id000206

000206-car___Check_Out_10-Mar-2023 18:30
000206-car___Check_In_10-Mar-2023 18:30
*/
#include<tc/tc_startup.h>
#include<tcinit\tcinit.h>
#include<tccore/item.h>
#include<tc/tc_macros.h>
#include<pom/pom/pom.h>
#include<tccore/aom_prop.h>
#include<tccore/aom.h>
#include<iostream>
#define AUDITLOGS "fnd0GeneralAuditLogs"
#define OBJECT_STRING "object_string"
using namespace std;
int ITK_user_main(int argc, char* argv[])
{

	tag_t tItemTag = NULLTAG;

	char *cValue = NULL;
	int iCount = 0;
	tag_t *tAuditTags = NULL;
	char *cUser = ITK_ask_cli_argument("-u=");
	char *cPass = ITK_ask_cli_argument("-p=");
	char *cGroup = ITK_ask_cli_argument("-g=");
	char *cItemId = ITK_ask_cli_argument("-itemid=");

	if (cUser != NULL && cPass != NULL && cGroup != NULL)
	{
		ITKCALL(ITK_init_module(cUser, cPass, cGroup));
		cout << "Login sucessful\n" << endl;
		ITKCALL(ITEM_find_item(cItemId, &tItemTag));
		if (tItemTag != NULLTAG)
		{
			AOM_ask_value_tags(tItemTag, AUDITLOGS, &iCount, &tAuditTags);
			if (iCount != 0 && tAuditTags != NULL)
			{
				cout << "Check in and check out history of Item id" << cItemId << endl << endl;;
				for (int i = 0; i < iCount; i++)
				{
					ITKCALL(AOM_ask_value_string(tAuditTags[i], OBJECT_STRING, &cValue));
					cout << cValue << "\n";
				}
			}
			else
			{
				cout << "No checkin checkout history found for item id "<< cItemId << endl;
			}
			
		}
		else
		{
			cout << "Item tag not found" << endl;
		}
		if (tAuditTags) MEM_free(tAuditTags);
		if (cValue) MEM_free(cValue);
	}
	else
	{
		cout << "Enter valid credentials" << endl;
	}
	return 0;
	ITK_exit_module(true);
}

