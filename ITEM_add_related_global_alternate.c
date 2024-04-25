#include<stdio.h>
#include<tcinit/tcinit.h>
#include<tc/tc_startup.h>
#include<bom/bom.h>
#include<ps/ps.h>
#include "Header.h"

#define LOGIN ITK_init_module("izn","izn","dba")

int ITK_user_main(int argc, char** argv)
{
	int iFail = 0;
	tag_t tag_item = NULLTAG;
	tag_t tag_selected_item = NULLTAG;
	tag_t tag_alternate_item = NULLTAG;

	iFail = LOGIN;
	if (iFail == 0)
	{
		printf("TC Login successful!\n---------------------\n\n");
		CHECK(ITEM_find_item("000249", &tag_selected_item));
		CHECK(ITEM_find_item("000321", &tag_alternate_item));
		tag_t array[] = { tag_alternate_item };
		CHECK(ITEM_add_related_global_alternates(tag_selected_item, 1, array));
		printf("Global alternate added!\n");
	}
	else
	{
		printf("TC Login Failed!\n-------------------------\n\n");
	}
	return ITK_ok;
}
















