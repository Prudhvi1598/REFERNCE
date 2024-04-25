#include<stdio.h>
#include<tcinit/tcinit.h>
#include<tc/tc_startup.h>
#include<tccore/item.h>
#include<bom/bom.h>
#include<ae/datasettype.h>
#include<ae/dataset.h>
#include "Header.h"

#define LOGIN ITK_init_module("izn","izn","dba")
#define ITEM_revise_operation           "Revise"
#define ITEM_saveas_operation           "SaveAs"
#define ITEM_copy_as_object             "copy_as_object"
#define ITEM_no_copy                    "no_copy"

int ITK_user_main(int argc, char** argv)
{
	int iFail = 0;
	tag_t t_parent_rev = NULLTAG;
	tag_t t_new_rev = NULLTAG;
	tag_t t_dataset2 = NULLTAG;
	tag_t t_dataset1 = NULLTAG;
	const char* copy_options[] = { "ITEM_no_copy", "ITEM_copy_as_object"};
	const char* relation_type[] = { "IMAN_specification","IMAN_specification" };
	const char* object_names[] = { "123","456"};
	char *text = NULL;
	const int option_count = 2;
	int n_deep_objects =0;
	tag_t *deep_copy_objects = NULL;

	iFail = LOGIN;
	if (iFail == 0)
	{
		printf("TC Login successful!\n---------------------\n\n");
		CHECK(ITEM_find_rev("000388", "A", &t_parent_rev));
		printf("Parent revision found!\n");
		CHECK(ITEM_copy_rev(t_parent_rev, "B", &t_new_rev));
		printf("New revision found!\n");
		CHECK(AE_find_dataset2("hi", &t_dataset1));
		CHECK(AE_find_dataset2("000388/A", &t_dataset2));
		const tag_t objects_tag[] = { t_dataset1,t_dataset2 };
		iFail=ITEM_perform_deepcopy_using(t_new_rev, ITEM_saveas_operation, t_parent_rev,option_count,objects_tag,copy_options,object_names,relation_type,&n_deep_objects,&deep_copy_objects);
		if (iFail != 0)
		{
			EMH_ask_error_text(iFail, &text);
			printf("%s\n\n", text);
		}
		printf("Deep copy rule performed");
	}
	else
	{
		printf("TC Login Failed!\n-------------------------\n\n");
	}
	return ITK_ok;
}
















