#include<stdio.h>
#include<tcinit/tcinit.h>
#include<tc/tc_startup.h>
#include<tccore/aom.h>
#include<tccore/item.h>
#include<ps/ps.h>
#include<bom/bom.h>

#define LOGIN ITK_init_module("izn","izn","dba")


int ITK_user_main(int argc, char** argv)
{
	int iFail = 0;
	tag_t tpitem = NULLTAG;
	tag_t tprev = NULLTAG;
	tag_t tBOMview = NULLTAG;
	tag_t tbvr = NULLTAG;
	tag_t twindow = NULLTAG;
	tag_t ttopline = NULLTAG;
	tag_t tnewline = NULLTAG;
	tag_t tchilditem = NULLTAG;
	tag_t tchildrev = NULLTAG;
	iFail = LOGIN;
	if (iFail == ITK_ok)
	{
		printf("TC Login Successful!\n\n");
		ITEM_find_item("000344", &tpitem);
		ITEM_find_rev("000344", "A", &tprev);
		BOM_create_window(&twindow);
		BOM_save_window(twindow);
		if (twindow != NULLTAG)
		{
			printf("Window created succesfully!\n\n");
			PS_create_bom_view(NULLTAG, "swamy", NULL, tpitem, &tBOMview);
			AOM_save_without_extensions(tpitem);
			AOM_save_without_extensions(tBOMview);
			if (tBOMview != NULLTAG)
			{
				printf("BOMview Created Successfully!\n\n");
				PS_create_bvr(tBOMview, "kumar", NULL, false, tprev, &tbvr);
				AOM_save_without_extensions(tbvr);
				if (tbvr != NULLTAG)
				{
					printf("BOMviewRevision created Successfully!\n\n");
					iFail = BOM_set_window_top_line(twindow, tpitem, tprev, tBOMview, &ttopline);
					if (iFail == ITK_ok)
					{
						printf("Top line created succesfully!\n\n");
						ITEM_find_item("000345", &tchilditem);
						ITEM_find_rev("000345", "A", &tchildrev);
						iFail = BOM_line_add(ttopline, tchilditem, tchildrev, NULLTAG, &tnewline);
						BOM_save_window(twindow);
						if (iFail == ITK_ok)
						{
							
							printf("Child line Added and BOMWindow Saved Succesfully!\n\n");
						}
						else
						{
							printf("child line cannot be added!");
						}
					}
					else
					{
						printf("Top line cannot be created!\n\n");
					}
				}
				else
				{
					printf("BOMview Revison not created!\n\n");
				}
			}
			else
			{
				printf("BOMview cannot be created!\n\n");
			}
		}
		else
		{
			printf("BOM Window not created!\n\n");
		}
	}
	else
	{
		printf("TC Login Unsuccessful");
	}
	return ITK_ok;
}