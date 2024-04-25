
/* check latest revision */


#include<stdio.h>
#include<tcinit/tcinit.h>
#include<tc/tc_startup.h>
#include<bom/bom.h>
#include<tccore/workspaceobject.h>

#define LOGIN ITK_init_module("izn","izn","dba")

int ITK_user_main(int argc, char** argv)
{
	int iFail = 0;
	int icount = 0;
	int irevcount = 0;
	tag_t trev = NULLTAG;
	tag_t titem = NULLTAG;
	tag_t *tstatuses = NULL;
	tag_t *trevlist = NULL;

	iFail = LOGIN;
	if (iFail == 0)
	{
		printf("TC Login successful!\n---------------------\n\n");
		ITEM_find_item("000365", &titem);
		ITEM_ask_latest_rev(titem, &trev);
		WSOM_ask_release_status_list(trev, &icount, &tstatuses);
		if (icount != 0)
		{
			printf("Latest Rev has release status!\n\n");
			ITEM_set_rev_description(trev, "SAI");
			ITEM_save_rev(trev);
			printf("Desc set successful!\n");
		}
		else
		{
			printf("Latest Rev does not has release status!\n\n");
			ITEM_list_all_revs(titem, &irevcount, &trevlist);
			printf("Total no of revisions are: %d\n\n", irevcount);
			if (irevcount > 2)
			{
				for (int i = 2; i < 4; i++)
				{
					ITEM_set_rev_description(trevlist[irevcount - i], "SAI");
					ITEM_save_rev(trevlist[irevcount - i]);
					printf("Desc set successful for %d position ItemRev\n",irevcount-i);
				}
			}
			else
			{
				ITEM_set_rev_description(trevlist[0], "SAI");
				ITEM_save_rev(trevlist[0]);
				printf("Desc set successful for 1st revision!\n");
			}
		}
	}
	else
	{
		printf("TC Login Failed!\n-------------------------\n\n");
	}
	return ITK_ok;
}
















