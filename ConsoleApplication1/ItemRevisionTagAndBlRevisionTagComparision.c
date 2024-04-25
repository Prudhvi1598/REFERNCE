#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<tc\emh.h>
#include<tccore\item.h>
#include<tccore\aom_prop.h>
#include<tccore\aom.h>
#include<tcinit\tcinit.h>
#include<tc/tc_startup.h>
#include<tccore/grm.h>
#include<bom/bom.h>
#include<fclasses/tc_string.h>
#define OBJECT_TYPE "object_type"
int check(int exp);
int check(int exp) {
	int iFail = 0;
	char *cError = NULL;
	if (exp != iFail)
	{
		EMH_ask_error_text(iFail, &cError);
		printf("The error is :%s\n", cError);
		exit(0);
	}
	return 0;
}
int ITK_user_main(int argc, char* argv[])
{
	tag_t tItem = NULLTAG;
	tag_t tRev = NULLTAG;
	int iCount = 0;
	tag_t *tBv = NULL;
	tag_t tWindow = NULLTAG;
	tag_t tBomLine = NULLTAG;
	tag_t tag = NULLTAG;
	check(ITK_init_module("hello", "hello", "dba"));
	ITEM_find_item("000360", &tItem);
	ITEM_find_revision(tItem, "A", &tRev);
	if (tRev != NULLTAG)
	{
		ITEM_list_bom_views(tItem, &iCount, &tBv);
			if (iCount != 0 && tBv != NULL)
			{
				for (int i = 0; i < iCount; i++)
				{
					BOM_create_window(&tWindow);
					if (tWindow != NULLTAG)
					{
						BOM_set_window_top_line(tWindow, tItem, tRev, tBv[i], &tBomLine);
						if (tBomLine != NULLTAG)
						{
							AOM_ask_value_tag(tBomLine, "bl_revision", &tag);
							if (tag != NULLTAG)
							{
								if (tag== tRev)
								{
									printf("compare sucess\n");
								}
								else
								{
									printf("compare unsucess\n");
								}
							}
						}
					}
				}
			}
	}
	return 0;
}