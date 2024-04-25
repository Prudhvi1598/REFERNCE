#include<stdio.h>
#include<tcinit/tcinit.h>
#include<tc/tc_startup.h>
#include<bom/bom.h>
#include "Header.h"

#define LOGIN ITK_init_module("izn","izn","dba")

int ITK_user_main(int argc, char** argv)
{
	int iFail = 0;
	int n_count = 0;
	tag_t tag_window1 = NULLTAG;
	tag_t tag_item1 = NULLTAG;
	tag_t tag_bomline1 = NULLTAG;
	tag_t *tag_children = NULLTAG;
	char **report_lines = NULL;

	iFail = LOGIN;
	if (iFail == 0)
	{
		printf("TC Login successful!\n---------------------\n\n");
		CHECK(BOM_create_window(&tag_window1));
		CHECK(ITEM_find_item("000248", &tag_item1));
		CHECK(BOM_set_window_top_line(tag_window1, tag_item1, NULLTAG, NULLTAG, &tag_bomline1));
		CHECK(BOM_line_ask_all_child_lines(tag_bomline1, &n_count, &tag_children));
		for (int i = 0; i < n_count; i++)
		{

		}
	
	}
	else
	{
		printf("TC Login Failed!\n-------------------------\n\n");
	}
	return ITK_ok;
}