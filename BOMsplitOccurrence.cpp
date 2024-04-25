#include<stdio.h>
#include<tcinit/tcinit.h>
#include<tc/tc_startup.h>
#include<bom/bom.h>

#define LOGIN ITK_init_module("izn","izn","dba")

int ITK_user_main(int argc, char** argv)
{
	int iFail = 0;
	int icount = 0;
	tag_t  twindow = NULLTAG;
	tag_t  *tchild = NULL;
	tag_t  tbomline = NULLTAG;
	tag_t  tviewtype = NULLTAG;
	tag_t  trev = NULLTAG;
	tag_t  ttopline = NULLTAG;
	iFail = LOGIN;
	if (iFail == 0)
	{
		printf("TC Login successful!\n----------------\n\n");
		BOM_create_window(&twindow);
		ITEM_find_rev("000273", "A", &trev);
		BOM_set_window_top_line(twindow, NULLTAG, trev, NULLTAG, &ttopline);
		if (ttopline != NULLTAG)
		{
			printf("Top line found!\n\n");
			BOM_line_ask_child_lines(ttopline, &icount, &tchild);
			printf("The NO of childs: %d\n\n", icount);
			tbomline = tchild[1];
			iFail = BOM_line_split_occurrence("2", tbomline);
			if (iFail == 0)
			{
				printf("Split occurrence!\n\n");
				BOM_save_window(twindow);
				return ITK_ok;
			}
			else
				printf("Split occurence failed!\n\n");
		}
		else
		{
			printf("Top line not found!\n\n");
		}
	}
	else
	{
		printf("TC Login Failed!\n----------------\n\n");
	}
	return ITK_ok;
}