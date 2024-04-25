#include<stdio.h>
#include<tcinit/tcinit.h>
#include<tc/tc_startup.h>
#include<ps/ps.h>
#include<bom/bom.h>
#include "Header.h"

#define LOGIN ITK_init_module("izn","izn","dba")

int ITK_user_main(int argc, char** argv)
{
	int iFail = 0;
	int n_count = 0;
	int n_count1 = 0;
	tag_t tag_window = NULLTAG;
	tag_t tag_item_rev = NULLTAG;
	tag_t tag_item = NULLTAG;
	tag_t tag_top_bom_line = NULLTAG;
	tag_t *tag_bvrs = NULLTAG;
	tag_t *tag_children = NULLTAG;

	iFail = LOGIN;
	if (iFail == 0)
	{
		printf("TC Login successful!\n---------------------\n\n");
		CHECK(BOM_create_window(&tag_window));
		CHECK(ITEM_find_rev("000248", "A",&tag_item_rev));
		CHECK(ITEM_find_item("000248",&tag_item));
		CHECK(BOM_set_window_top_line(tag_window, tag_item, tag_item_rev, NULLTAG, &tag_top_bom_line));
		CHECK(ITEM_rev_list_all_bom_view_revs(tag_item_rev, &n_count, &tag_bvrs));
		CHECK(BOM_line_ask_all_child_lines(tag_top_bom_line, &n_count1, &tag_children));
		CHECK(PS_move_occurrence_to(tag_bvrs[0],tag_children[4], 0));
		BOM_save_window(tag_window);
	}
	else
	{
		printf("TC Login Failed!\n-------------------------\n\n");
	}
	return ITK_ok;
}