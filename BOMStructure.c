#include<stdio.h>
#include<tcinit/tcinit.h>
#include<tc/tc_startup.h>
#include<bom/bom.h>
#include<ps/ps.h>
#include<tccore/aom.h>

#define LOGIN ITK_init_module("izn","izn","dba")

int ITK_user_main(int argc, char** argv)
{
	int iFail = 0;
	tag_t twindow = NULLTAG;
	tag_t tprev = NULLTAG;
	tag_t tbvtag = NULLTAG;
	tag_t tbvrtag= NULLTAG;
	tag_t trev1 = NULLTAG;
	tag_t trev2 = NULLTAG;
	tag_t trev3 = NULLTAG;
	tag_t trev4 = NULLTAG;
	tag_t ttopline= NULLTAG;
	tag_t tnewline= NULLTAG;
	tag_t tnewline1= NULLTAG;
	tag_t tnewline2= NULLTAG;
	tag_t tnewline3= NULLTAG;
	tag_t tnewline4= NULLTAG;

	iFail = LOGIN;
	if (iFail == 0)
	{
		printf("TC Login successful!\n---------------------\n\n");
		BOM_create_window(&twindow);
		if (twindow != NULLTAG)
		{
			printf("Window created!\n\n");
			ITEM_find_rev("000357", "A", &tprev);
			PS_create_bv_with_bvr(NULL, NULL, NULLTAG, "bvr", NULL, false,tprev,&tbvtag,&tbvrtag);
			BOM_save_window(twindow);
			if (tbvtag != NULLTAG && tbvrtag != NULLTAG)
			{
				printf("BOMview and BVR created!\n\n");
				BOM_set_window_top_line(twindow, NULLTAG, tprev, tbvtag, &ttopline);
				BOM_save_window(twindow);
				if (ttopline != NULLTAG)
				{
					printf("Top line created!\n\n");

					ITEM_find_rev("000351","A", &trev1);
					BOM_line_add(ttopline, NULLTAG,trev1,NULLTAG, &tnewline1);

					ITEM_find_rev("000352", "A", &trev2);
					BOM_line_add(ttopline, NULLTAG,trev2, NULLTAG, &tnewline2);
				
					ITEM_find_rev("000353", "A", &trev3);
					BOM_line_add(ttopline, NULLTAG, trev3, NULLTAG, &tnewline3);
					
					ITEM_find_rev("000354", "A", &trev4);
					iFail=BOM_line_add(ttopline, NULLTAG, trev4, NULLTAG, &tnewline4);
					
					BOM_save_window(twindow);
					if (iFail == 0)
						printf("Structure created successfull!\n\n");
					else
						printf("Structure creation failed!\n\n");

				}
				else
				{
					printf("Top line not created!\n\n");
				}
			}
			else
			{
				printf("BOMview and BVR not created!\n\n");
			}
		}
		else
		{
			printf("Window not created!\n\n");
		}
	}
	else
	{
		printf("TC Login Failed!\n-------------------------\n\n");
	}
	return ITK_ok;
}