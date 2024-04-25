#include<stdio.h>
#include<tcinit/tcinit.h>
#include<tc/tc_startup.h>
#include<tccore/releasestatus.h>
#include<tccore/workspaceobject.h>
#include<bom/bom.h>
#include<epm/epm.h>
#include "Header.h"

#define LOGIN ITK_init_module("izn","izn","dba")

int ITK_user_main(int argc, char** argv)
{
	int iFail = 0;
	int icount = 0;
	int is_released = 0;
	char *char_id = NULL;
	tag_t titem = NULLTAG;
	tag_t *tag_rev_list = NULL;
	tag_t tag_released = NULLTAG;


	iFail = LOGIN;
	if (iFail == 0)
	{
		printf("TC Login successful!\n---------------------\n\n");
		ITEM_find_item("000321", &titem);
		CHECK(ITEM_list_all_revs(titem, &icount, &tag_rev_list));
		if (icount != 0)
		{
			printf("Item Revisions found!\n\n");
			for (int i = 0; i < icount; i++)
			{
				EPM_ask_if_released(tag_rev_list[i], &is_released);
				if (is_released == 1)
					tag_released = tag_rev_list[i];
			}
			WSOM_ask_id_string(tag_released, &char_id);
			printf("The latest released Revision is:%s\n\n", char_id);
		}
		else
		{
			printf("Item Revisions not found!\n\n");
		}
	}
	else
	{
		printf("TC Login Failed!\n-------------------------\n\n");
	}
	return ITK_ok;
}
