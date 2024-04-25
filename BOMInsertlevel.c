
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
	tag_t  trev = NULLTAG;
	tag_t  tnewrev = NULLTAG;
	tag_t  ttopline = NULLTAG;
	iFail = LOGIN;
	if (iFail == 0)
	{
		printf("TC Login successful!\n----------------\n\n");
		BOM_create_window(&twindow);
		BOM_save_window(twindow);
		ITEM_find_rev("000359", "A", &trev);
		BOM_set_window_top_line(twindow, NULLTAG, trev, NULLTAG, &ttopline);
		if (ttopline != NULLTAG)
		{
			printf("Top line found!\n\n");
			BOM_line_ask_child_lines(ttopline, &icount, &tchild);
			printf("The NO of childs: %d\n\n", icount);
			ITEM_find_rev("000364", "A", &tnewrev);
			iFail = BOM_line_insert_level(1, &tchild[1], tnewrev, NULLTAG);
			if (iFail == 0)
			{
				printf("level Inserted!\n\n");
				BOM_save_window(twindow);
			}
			else
				printf("Insert level failed!\n\n");
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


