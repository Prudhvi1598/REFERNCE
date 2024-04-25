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
	tag_t tag_window = NULLTAG;
	tag_t tag_item = NULLTAG;
	tag_t tag_sub = NULLTAG;
	tag_t tag_top_bom_line = NULLTAG;
	tag_t tag_new_line = NULLTAG;
	tag_t *tag_children = NULLTAG;



	iFail = LOGIN;
	if (iFail == 0)
	{
		printf("TC Login successful!\n---------------------\n\n");
		CHECK(BOM_create_window(&tag_window));
		CHECK(ITEM_find_item("000248", &tag_item));
		CHECK(ITEM_find_item("000273", &tag_sub));
		CHECK(BOM_set_window_top_line(tag_window, tag_item, NULLTAG, NULLTAG, &tag_top_bom_line));
		CHECK(BOM_line_ask_child_lines(tag_top_bom_line, &n_count, &tag_children));
		CHECK(BOM_line_add_substitute(tag_children[4], tag_sub, NULLTAG, NULLTAG, &tag_new_line));
		AOM_save_without_extensions(tag_new_line);
		BOM_save_window(tag_window);
		printf("Substitute added!\n");
	}
	else
	{
		printf("TC Login Failed!\n-------------------------\n\n");
	}
	return ITK_ok;
}
















