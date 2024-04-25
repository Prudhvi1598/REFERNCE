#include<stdio.h>
#include<tcinit/tcinit.h>
#include<tc/tc_startup.h>
#include<tccore/item.h>
#include<tccore/workspaceobject.h>
#include<bom/bom.h>

#define LOGIN ITK_init_module("izn","izn","dba")

#define WSO_where_ref_any_depth   -1

int ITK_user_main(int argc, char** argv)
{
	int iFail = 0;
	int ireferences = 0;
	int *ilevels = 0;
	tag_t titem = NULLTAG;
	tag_t *treferences = NULLTAG;
	char **crelations = NULL;
	char *crelation = NULL;
	char *creference = NULL;
	iFail = LOGIN;
	if (iFail == 0)
	{
		printf("TC Login successful!\n---------------------\n\n");
		ITEM_find_rev("000328", "A", &titem);
		iFail = WSOM_where_referenced2(titem, WSO_where_ref_any_depth, &ireferences, &ilevels, &treferences, &crelations);
		if (iFail == 0)
		{
			printf("Index\tlevel\treferencer\t\t\t\trelation\n\n");
			for (int i = 0; i < ireferences; i++)
			{
				WSOM_ask_object_id_string(treferences[i], &creference);
				printf("%d\t%d\t%s\t\t\t\t%s\n", i, ilevels[i], creference, crelations[i]);
				char *creference = NULL;
				char *crelation = NULL;
			}
			MEM_free(creference);
			MEM_free(crelations);
		}
		else
		{
			printf("Where reference found failed!\n\n");
		}

	}
	else
	{
		printf("TC Login Failed!\n-------------------------\n\n");
	}
	return ITK_ok;
}











