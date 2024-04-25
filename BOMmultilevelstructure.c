#include<stdio.h>
#include<tcinit/tcinit.h>
#include<tc/tc_startup.h>
#include<bom/bom.h>
#include<ps/ps.h>
#include<tccore/aom.h>

#define LOGIN ITK_init_module("izn","izn","dba")

tag_t twindow = NULLTAG;

/* bv and bvr function */
tag_t bv_bvr(char *crev)
{
	tag_t tprev = NULLTAG;
	tag_t tbvtag = NULLTAG;
	tag_t tbvrtag = NULLTAG;
	tag_t ttopline = NULLTAG;

	BOM_create_window(&twindow);
	BOM_save_window(twindow);
	if (twindow != NULLTAG)
	{
		printf("Window created!\n\n");
		ITEM_find_rev(crev, "A", &tprev);
		PS_create_bv_with_bvr(NULL, NULL, NULLTAG, NULL, NULL, false, tprev, &tbvtag, &tbvrtag);
		BOM_save_window(twindow);
		if (tbvtag != NULLTAG && tbvrtag != NULLTAG)
		{
			printf("BOMview and BVR created!\n\n");
			BOM_set_window_top_line(twindow, NULLTAG, tprev, tbvtag, &ttopline);
			BOM_save_window(twindow);
			if (ttopline != NULLTAG)
				printf("Top line created!\n\n");
			else
				printf("Top line not created!\n\n");
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
		return ttopline;
}

/* BOMline function */
void addbomline(char *crev,tag_t tbomline)
{
	int iFail = 0;
	tag_t trev = NULLTAG;
	tag_t tnewline = NULLTAG;

	ITEM_find_rev(crev, "A", &trev);
	iFail=BOM_line_add(tbomline, NULLTAG, trev, NULLTAG, &tnewline);
	BOM_save_window(twindow);
	if (iFail == 0)
		printf("BOM line added!\n");
	else
		printf("BOM line cannot be added!\n");

}

/* main function*/
int ITK_user_main(int argc, char* argv[])
{
	int iFail = 0;
	tag_t ttopline = NULLTAG;
	tag_t ttopline1 = NULLTAG;
	tag_t ttopline2 = NULLTAG;


	iFail = LOGIN;
	if (iFail == 0)
	{
		printf("TC Login successful!\n---------------------\n\n");
		ttopline = bv_bvr("000357");
		addbomline("000351", ttopline);
		addbomline("000354", ttopline);
		ttopline1 = bv_bvr("000351");
		addbomline("000352", ttopline1);
		ttopline2 = bv_bvr("000354");
		addbomline("000353", ttopline2);

	}
	else
	{
		printf("TC Login Failed!\n-------------------------\n\n");
	}
	return ITK_ok;
}