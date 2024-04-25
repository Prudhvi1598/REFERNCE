#include<stdio.h>
#include<tcinit/tcinit.h>
#include<tc/tc_startup.h>
#include<bom/bom.h>
#include "Header.h"

#define BOM_compare_singlelevel          0
#define BOM_compare_output_report        4
#define LOGIN ITK_init_module("izn","izn","dba")

int ITK_user_main(int argc, char** argv)
{
	int iFail = 0;
	int report_length = 0;
	tag_t tag_window1 = NULLTAG;
	tag_t tag_window2 = NULLTAG;
	tag_t tag_item1 = NULLTAG;
	tag_t tag_item2 = NULLTAG;
	tag_t tag_bomline1 = NULLTAG;
	tag_t tag_bomline2 = NULLTAG;
	tag_t *tag_report_items = NULLTAG;
	char **report_lines = NULL;

	iFail = LOGIN;
	if (iFail == 0)
	{
		printf("TC Login successful!\n---------------------\n\n");
		CHECK(BOM_create_window(&tag_window1));
		CHECK(BOM_create_window(&tag_window2));
		CHECK(ITEM_find_item("000248", &tag_item1));
		CHECK(ITEM_find_item("000347", &tag_item2));
		CHECK(BOM_set_window_top_line(tag_window1, tag_item1, NULLTAG, NULLTAG, &tag_bomline1));
		CHECK(BOM_set_window_top_line(tag_window2, tag_item2, NULLTAG, NULLTAG, &tag_bomline2));
		CHECK(BOM_compare(tag_bomline1, tag_bomline2, BOM_compare_singlelevel, BOM_compare_output_report));
		CHECK(BOM_compare_report(tag_bomline1, &report_length, &report_lines, &tag_report_items));
		printf("length:%d\n\n", report_length);
		for (int i = 0; i < report_length;i++)
		{
			printf("%s\n", report_lines[i]);
		}
	}
	else
	{
		printf("TC Login Failed!\n-------------------------\n\n");
	}
	return ITK_ok;
}