#include<stdio.h>
#include<tcinit/tcinit.h>
#include<tc/tc_startup.h>
#include<bom/bom.h>
#include<tccore/aom.h>
#include<tc/emh.h>

#define LOGIN ITK_init_module("izn","izn","dba")

int ITK_user_main(int argc, char** argv)
{
	int iFail = 0;
	int icount = 0;
	int ndates = 0;
	char *ctext = NULL;
	const date_t date = NULLDATE;
	const char *crange = "1-5";
	tag_t twindow = NULLTAG;
	tag_t teffectivity = NULLTAG;
	tag_t tenditem = NULLTAG;
	tag_t tenditemrev = NULLTAG;
	tag_t ttopbomline = NULLTAG;
	tag_t *tchildren = NULLTAG;


	iFail = LOGIN;
	if (iFail == 0)
	{
		printf("TC Login successful!\n---------------------\n\n");
		BOM_create_window(&twindow);
		if (twindow != NULLTAG)
		{
			printf("Window opened!\n\n");
			ITEM_find_item("000357", &tenditem);
			ITEM_find_rev("000357", "A", &tenditemrev);
			BOM_set_window_top_line(twindow,tenditem,NULLTAG,NULLTAG,&ttopbomline);
			if (ttopbomline != NULLTAG)
			{
				printf("Top bomline found!\n\n");
				BOM_line_ask_all_child_lines(ttopbomline, &icount, &tchildren);
				printf("The No of child lines:%d\n\n", icount);
				if (tchildren != NULLTAG)
				{
					printf("All child lines found!\n\n");
					BOM_create_occurrence_effectivity(tchildren[0], NULL,tenditem, NULLTAG, crange, 0, &date, EFFECTIVITY_closed, false, &teffectivity);
					AOM_save_without_extensions(teffectivity);
					BOM_save_window(twindow);
					if (teffectivity != NULLTAG)
						printf("Occurrence Effectivity set successful!\n\n");
					else
					{
						EMH_ask_error_text(teffectivity, &ctext);
						printf("%s\n\n",ctext);
					}
						
				}
				else
				{
					printf("child lines not found!\n\n");
				}
			}
			else
			{
				printf("Top bomline not found!\n\n");
			}
		}
		else
		{
			printf("Window opened failed!\n\n");
		}

	}
	else
	{
		printf("TC Login Failed!\n-------------------------\n\n");
	}
	return ITK_ok;
}
















