#include<stdio.h>
#include<tcinit/tcinit.h>
#include<tc/tc_startup.h>
#include<tccore/aom.h>
#include<tccore/item.h>
#include<ps/ps.h>
#include<bom/bom.h>

#define LOGIN ITK_init_module("infodba","infodba","dba")


int ITK_user_main(int argc, char** argv)
{
	int iFail = 0;
	tag_t titem = NULLTAG;
	tag_t trev = NULLTAG;
	tag_t tviewtype = NULLTAG;
	tag_t tBOMview = NULLTAG;
	tag_t tbvr = NULLTAG;
	tag_t twindow = NULLTAG;
	tag_t ttopline = NULLTAG;
	tag_t tnewline = NULLTAG;
	tag_t tViewType = NULLTAG;
	iFail = LOGIN;
	if (iFail == ITK_ok)
	{
		printf("TC Login Successful!\n\n");
		ITEM_find_item("000336", &titem);
		ITEM_find_rev("000336", "A", &trev);
		PS_create_bom_view(tViewType, "swamy", "", titem, &tBOMview);
		if (tBOMview!=NULLTAG)
		{
			AOM_save_without_extensions(tBOMview);
			printf("BOMview Created Successfully!\n\n");
			PS_create_bvr(tBOMview, "kumar", "", false, trev, &tbvr);
			if (tbvr!=NULLTAG)
			{
				AOM_save_without_extensions(tbvr);
				printf("BOMviewRevision created Successfully!\n\n");
				BOM_create_window(&twindow);
				if (twindow != NULLTAG)
				{
					printf("Window created succesfully!\n\n");
					iFail = BOM_set_window_top_line_bvr(twindow, tbvr, &ttopline);
					if (iFail == ITK_ok)
					{
						printf("Top line created succesfully!\n\n");
						ITEM_find_rev("000337", "A", &trev);
						iFail = BOM_line_add(ttopline, NULLTAG, trev, tBOMview, &tnewline);
						if (iFail == ITK_ok)
						{
							BOM_save_window(twindow);
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
					printf("BOM Window not created!\n\n");
				}
			}
			else
			{
				printf("BOMviewRevision cannot be created!\n\n");
			}
		}
		else
		{
			printf("BOMview cannot be created!\n\n");
		}
	}
	else
	{
		printf("TC Login Unsuccessful");
	}
	return ITK_ok;
}