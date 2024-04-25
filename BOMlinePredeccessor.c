#include<stdio.h>
#include<string.h>
#include<tcinit/tcinit.h>
#include<tc/tc_startup.h>
#include<bom/bom.h>
#include<tccore/workspaceobject.h>

#define LOGIN ITK_init_module("izn","izn","dba")
tag_t twindow = NULLTAG;

tag_t bv_bvr(char *crev)
{
	tag_t tprev = NULLTAG;
	tag_t ttopline = NULLTAG;

	BOM_create_window(&twindow);
	if (twindow != NULLTAG)
	{
		printf("Window opened!\n\n");
		ITEM_find_rev(crev, "A", &tprev);
		if (tprev != NULLTAG)
		{
			BOM_set_window_top_line(twindow, NULLTAG, tprev, NULLTAG, &ttopline);
			BOM_save_window(twindow);
			if (ttopline != NULLTAG)
				printf("Top line found!\n\n");
			else
				printf("Top line not found!\n\n");
		}
		else
		{
			printf("ItemRev not found!\n\n");
		}
	}
	else
	{
		printf("Window not opened!\n\n");
	}
	return ttopline;
}

int ITK_user_main(int argc, char** argv)
{
	int iFail = 0;
	int icount = 0;
	
	tag_t tpredecessor = NULLTAG;
	tag_t ttopline = NULLTAG;
	tag_t *tchilds = NULLTAG;
	char *crevname = NULL;

	iFail = LOGIN;
	if (iFail == 0)
	{
		printf("TC Login successful!\n---------------------\n\n");
		ttopline = bv_bvr("000357");
		BOM_line_ask_all_child_lines(ttopline, &icount, &tchilds);
		printf("The number of childs are: %d\n\n", icount);
		for (int i = 0; i < icount; i++)
		{
			if (i == 1)
			{
				ITEM_find_rev("000165", "A", &tpredecessor);
				if (tpredecessor != NULLTAG)
				{
					printf("Predescessor found!\n\n");
					iFail = BOM_line_add_predecessor(tchilds[i], tpredecessor);
					BOM_save_window(twindow);
					if (iFail == 0)
						printf("Predecessor added succesful!\n");
					else
						printf("Predecessor added Unsuccessful!\n");
				}
				else
				{
					printf("Predescessor not found!\n\n");
				}
				
			}
		}
	}
	else
	{
		printf("TC Login Failed!\n-------------------------\n\n");
	}
	return ITK_ok;
}
















