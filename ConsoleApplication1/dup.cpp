/*
-------------changing overlay icon based on property value-------------
itemtype =object2

		-u=hello -p=hello -g=dba -itemid=000 -desc=Y
*/

#include<iostream>
#include<tc/tc_startup.h>
#include<tcinit\tcinit.h>
#include<tc/emh.h>
#include<tccore/item.h>
#include<tccore/aom.h>
#include<tccore/aom_prop.h>
#define OBJECT_DESC "object_desc"
using namespace std;
int check(int exp);
int ITK_user_main(int argc, char* argv[])
{
	tag_t tItemTag = NULLTAG;
	int iCount = 0;
	tag_t *tBvList = NULL;
	tag_t tWindow = NULLTAG;
	tag_t tBomLine = NULLTAG;
	char *cUser = ITK_ask_cli_argument("-u=");
	char *cPass = ITK_ask_cli_argument("-p=");
	char *cGroup = ITK_ask_cli_argument("-g=");
	char *cItemId = ITK_ask_cli_argument("-itemid=");
	//char *cDescValue = ITK_ask_cli_argument("-desc=");
	char *cValue = NULL;
	if (cUser != NULL && cPass != NULL && cGroup != NULL)
	{
		check(ITK_init_module(cUser, cPass, cGroup));
		cout << "Login sucessful\n" << endl;

		check(ITEM_find_item(cItemId, &tItemTag));
		if (tItemTag != NULLTAG)
		{
			AOM_UIF_ask_value(tItemTag, "object_string", &cValue);
			cout << "value is" << cValue <<endl;
			
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
/*
AOM_refresh(tItemTag, true);
			check(AOM_set_value_string(tItemTag, OBJECT_DESC, cDescValue));
			cout << "value updated" << endl;
			AOM_save_without_extensions(tItemTag);
			AOM_refresh(tItemTag, false);




#include <tccore/aom.h>
#include <tccore/aom_prop.h>
#include <tccore/custom.h>
#include <tccore/item.h>
#include <tccore/item_revision.h>
#include <tc/emh.h>
#include <tc/folder.h>
#include <tc/folder_contents.h>
#include <tc/preferences.h>
#include <tc/tag.h>
#include <tc/tc.h>
void set_object_icon(const char *object_type, const char *icon_path)
{
	tag_t icon_tag = NULLTAG;
	tag_t icon_file_tag = NULLTAG;
	tag_t icon_prop_tag = NULLTAG;
	char *icon_prop_name = NULL;
	char *icon_file_name = NULL;
	ITKCALL(AOM_get_icon_tag(object_type, &icon_tag));
	ITKCALL(AOM_ask_value_tag(icon_tag, "image_file", &icon_prop_tag));
	ITKCALL(AOM_get_value_string(icon_prop_tag, &icon_prop_name));
	ITKCALL(AOM_get_value_tag(icon_prop_tag, &icon_file_tag));
	ITKCALL(AOM_ask_value_string(icon_file_tag, "name", &icon_file_name));
	if (strcmp(icon_prop_name, "image_file") == 0)
	{
		ITKCALL(AOM_lock(icon_prop_tag));
		ITKCALL(AOM_set_value_string(icon_file_tag, "name", icon_path));
		ITKCALL(AOM_save(icon_prop_tag));
		ITKCALL(AOM_unlock(icon_prop_tag));
	}
}
*/