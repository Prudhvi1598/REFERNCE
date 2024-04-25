/*
---------AFTER UPDATING ANY PROPERTY THE LAST MODIFIED DATE PROPERTY SHOULD NOT GET UPDATED---------



INPUT = -u=hello -p=hello -g=dba -itemid=<itemid> -desc=<desc>


*/


#include<tc/tc_startup.h>
#include<tcinit\tcinit.h>
#include<tccore/item.h>
#include<tc/tc_macros.h>
#include<pom/pom/pom.h>
#include<tccore/aom_prop.h>
#include<tccore/aom.h>
#include<iostream>
#define PROP "last_mod_date"
#define DESC "object_desc"
using namespace std;
int ITK_user_main(int argc, char* argv[])
{

	tag_t tItemTag = NULLTAG;
	
	char *cValue = NULL;
	char *cValue1 = NULL;
	char *cUser = ITK_ask_cli_argument("-u=");
	char *cPass = ITK_ask_cli_argument("-p=");
	char *cGroup = ITK_ask_cli_argument("-g=");
	char *cItemId = ITK_ask_cli_argument("-itemid=");
	char *cDesc = ITK_ask_cli_argument("-desc=");
	if (cUser != NULL && cPass != NULL && cGroup != NULL)
	{
		ITKCALL(ITK_init_module(cUser, cPass, cGroup));
		cout << "1Login sucessful\n" << endl;
		ITKCALL(ITEM_find_item(cItemId, &tItemTag));
		if (tItemTag != NULLTAG)
		{
			ITKCALL(AOM_refresh(tItemTag, true));
			ITKCALL(AOM_UIF_ask_value(tItemTag, PROP, &cValue));//last modified date before modifying any property
			if (cValue != NULL)
			{
				cout << "Date before modifying any property" << cValue << endl;
				ITKCALL(AOM_set_value_string(tItemTag, DESC, cDesc));//updating desciption
				cout << "Property set" << endl;
				ITKCALL(POM_set_env_info(POM_bypass_attr_update, false, 0, 0, NULLTAG, NULL));
				ITEM_save_item(tItemTag);
				cout << "Item saved" << endl;
				ITKCALL(AOM_UIF_ask_value(tItemTag, PROP, &cValue1));//last modified date after modifying any property
				cout << "Date after modifying any property" << cValue1 << endl;
				AOM_refresh(tItemTag, false);
			}
		}
		else
		{
			cout << "Item tag not found" << endl;
		}
	}
	else
	{
		cout << "Enter valid credentials" << endl;
	}
	return 0;
	ITK_exit_module(true);
}