#include<stdio.h>
#include<tcinit/tcinit.h>
#include<tc/tc_startup.h>
#include<bom/bom.h>
#include<tccore/grm.h>
#include "Header.h"

#define LOGIN ITK_init_module("izn","izn","dba")

int ITK_user_main(int argc, char** argv)
{
	int iFail = 0;
	int n_count = 0;
	int n_relation = 0;
	tag_t tag_relation_type=NULLTAG;
	tag_t tag_rev=NULLTAG;
	tag_t *tag_secondary_objects =NULL;
	GRM_relation_t *tag_grm_relation =NULL;

	iFail = LOGIN;
	if (iFail == 0)
	{
		printf("TC Login successful!\n---------------------\n\n");
		CHECK(ITEM_find_rev("000327", "A", &tag_rev));
		CHECK(GRM_list_secondary_objects(tag_rev,NULLTAG,&n_count, &tag_grm_relation));
		printf("The no of secondary objects:%d\n", n_count);
		for (int i = 0; i < n_count; i++)
		{
				GRM_delete_relation(tag_grm_relation[i].the_relation);
		}
		printf("Secondary objects Deleted\n\n");
	}
	else
	{
		printf("TC Login Failed!\n-------------------------\n\n");
	}
	return ITK_ok;
}