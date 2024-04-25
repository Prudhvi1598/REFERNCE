#include<stdio.h>
#include<tcinit/tcinit.h>
#include<tc/tc_startup.h>
#include<tccore/aom_prop.h>
#include<tccore/releasestatus.h>
#include<bom/bom.h>
#include "Header.h"

#define LOGIN ITK_init_module("izn","izn","dba")

int ITK_user_main(int argc, char** argv)
{
	int iFail = 0;
	int icount = 0;
	int n_values = 0;
	tag_t *tag_rel_values;
	tag_t *tag_list;
	tag_t tag_new_rev;
	char *rel_status_type;
	char *char_new_rev=NULL;
	char *char_rev_name=NULL;


	iFail = LOGIN;
	if (iFail == 0)
	{
		printf("TC Login successful!\n---------------------\n\n");
		CHECK(ITEM_extent_rev(&icount, &tag_list));
		for (int i = 0; i < icount; i++)
		{
			CHECK(AOM_ask_value_tags(tag_list[i], "release_status_list", &n_values, &tag_rel_values));
			CHECK(AOM_ask_name(tag_list[i], &char_rev_name));
			printf("%s\n", char_rev_name);
			if (n_values != 0)
			{
				for (int j = 0; j < n_values; j++)
				{
					CHECK(RELSTAT_ask_release_status_type(tag_rel_values[i], &rel_status_type));
					if (strcmp(rel_status_type, "TCM Released") == 0)
					{
						CHECK(ITEM_copy_rev(tag_list[i], NULL, &tag_new_rev));
						CHECK(ITEM_save_rev(tag_new_rev));
						CHECK(AOM_ask_name(tag_new_rev, &char_new_rev));
						printf("The Revised Object is: %s\n", char_new_rev);
						char *char_new_rev = NULL;
					}
				}
			}
		}
	}
	else
	{
		printf("TC Login Failed!\n-------------------------\n\n");
	}
	return ITK_ok;
}
















