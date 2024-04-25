#include<stdio.h>
#include<tcinit/tcinit.h>
#include<tc/tc_startup.h>
#include<bom/bom.h>
#include<tccore/aom.h>
#include<tccore/aom_prop.h>
#include<tc/emh.h>
#include<ps/ps.h>

#define LOGIN ITK_init_module("izn","izn","dba")

int ITK_user_main(int argc, char** argv)
{
	int iFail = 0;
	int icount = 0;
	int ndates = 0;
	char *ctext = NULL;
	const char *crange = "1-5";
	tag_t twindow = NULLTAG;
	tag_t teffectivity = NULLTAG;
	tag_t tenditem = NULLTAG;
	tag_t tenditemrev = NULLTAG;
	tag_t ttopbomline = NULLTAG;
	tag_t *tchildren = NULLTAG;
	tag_t *tbvrs = NULLTAG;
	tag_t tbvr = NULLTAG;
	tag_t tocc = NULLTAG;


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
			BOM_set_window_top_line(twindow, tenditem, NULLTAG, NULLTAG, &ttopbomline);
			BOM_save_window(twindow);
			if (ttopbomline != NULLTAG)
			{
				printf("Top bomline found!\n\n");
				BOM_line_ask_all_child_lines(ttopbomline, &icount, &tchildren);
				printf("The No of child lines:%d\n\n", icount);
				icount = 0;
				if (tchildren != NULLTAG)
				{
					printf("All child lines found!\n\n");
					ITEM_rev_list_all_bom_view_revs(tenditemrev, &icount, &tbvrs);
					if (icount != 0 && tbvrs != NULLTAG)
					{
						printf("top bvr found!\n\n");
						AOM_ask_value_tag(tchildren[0], "bl_occurrence", &tocc);
						AOM_save_without_extensions(tocc);
						PS_occ_eff_create(tbvrs[0], tocc, &teffectivity);
						AOM_save_without_extensions(teffectivity);
						AOM_save_without_extensions(tbvrs[0]);
						if (teffectivity != NULLTAG)
						{
							printf("Occ Eff created!\n\n");
							PS_occ_eff_set_enditemrev(tbvrs[0], tocc, teffectivity, tenditemrev);
							PS_occ_eff_set_unit_range(tbvrs[0], tocc, teffectivity, "1-5", true);
							AOM_save_without_extensions(teffectivity);
							AOM_save_without_extensions(tocc);
							BOM_save_window(twindow);
						}
						else
						{
							printf("Occ Eff not created!\n\n");
						}
					}
					else
					{
						printf("child lines not found!\n\n");
					}
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








