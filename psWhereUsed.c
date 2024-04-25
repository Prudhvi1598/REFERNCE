#include<stdio.h>
#include<tcinit/tcinit.h>
#include<tc/tc_startup.h>
#include<tccore/item.h>
#include<tccore/workspaceobject.h>
#include<bom/bom.h>
#include<ps/ps.h>

#define LOGIN ITK_init_module("izn","izn","dba")
#define  PS_where_used_all_levels   -1

int ITK_user_main(int argc, char** argv)
{
	int iFail = 0;
	int  n_parents = 0;
	int *ilevels = 0;
	tag_t titem = NULLTAG;
	tag_t *tparents = NULLTAG;
	char *cstring = NULL;
	iFail = LOGIN;
	if (iFail == 0)
	{
		printf("TC Login successful!\n---------------------\n\n");
		ITEM_find_rev("000276", "A", &titem);
		iFail = PS_where_used_all(titem, PS_where_used_all_levels, &n_parents, &ilevels, &tparents);
		if (iFail == 0)
		{
			printf("Index\tlevel\tparent\n\n");
			for (int index = 0; index < n_parents; index++)
			{
				WSOM_ask_object_id_string(tparents[index], &cstring);
				printf("%d\t%d\t%s\n", index, ilevels[index], cstring);
				char creference = NULL;
			}
			MEM_free(ilevels);
			MEM_free(tparents);
		}
		else
		{
			printf("Where used found failed!\n\n");
		}

	}
	else
	{
		printf("TC Login Failed!\n-------------------------\n\n");
	}
	return ITK_ok;
}