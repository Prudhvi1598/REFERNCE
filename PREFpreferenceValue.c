#include<stdio.h>
#include<tcinit/tcinit.h>
#include<tc/tc_startup.h>
#include<tc/preferences.h>
#include<bom/bom.h>
#include "Header.h"

#define LOGIN ITK_init_module("izn","izn","dba")

int ITK_user_main(int argc, char** argv)
{
	int iFail = 0;
	char *charvalue;
	tag_t titem = NULLTAG;
	iFail = LOGIN;
	if (iFail == 0)
	{
		printf("TC Login successful!\n---------------------\n\n");
		CHECK(PREF_ask_char_value("AIE_named_refs_to_export", 0, &charvalue));
		CHECK(ITEM_find_item("000321", &titem));
		ITEM_set_description(titem, charvalue);
		ITEM_save_item(titem);
		printf("The Value:%s set to Obj desc.\n", charvalue);
	}
	else
	{
		printf("TC Login Failed!\n-------------------------\n\n");
	}
	return ITK_ok;
}
















