
#include<stdio.h>
#include<tcinit/tcinit.h>
#include<tc/tc_startup.h>
#include<tccore/item.h>
#include<bom/bom.h>
#include "Header.h"

#define LOGIN ITK_init_module("izn","izn","dba")

int ITK_user_main(int argc, char** argv)
{
	int iFail = 0;
	tag_t t_parent_rev = NULLTAG;
	tag_t t_new_rev = NULLTAG;
	int n_objects[1] = { 1 };
	tag_t *deep_copy_objects = NULL;

	iFail = LOGIN;
	if (iFail == 0)
	{
		printf("TC Login successful!\n---------------------\n\n");
		CHECK(ITEM_find_rev("000388", "A", &t_parent_rev));
		printf("Parent revision found!\n");
		CHECK(ITEM_copy_rev(t_parent_rev, "B", &t_new_rev));
		printf("New revision found!\n");
		CHECK(ITEM_perform_deepcopy(t_new_rev, ITEM_revise_operation, t_parent_rev, n_objects, &deep_copy_objects));
		printf("Deep copy rule performed");
	}
	else
	{
		printf("TC Login Failed!\n-------------------------\n\n");
	}
	return ITK_ok;
}