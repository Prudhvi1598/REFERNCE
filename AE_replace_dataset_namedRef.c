#include<stdio.h>
#include<tcinit/tcinit.h>
#include<tc/tc_startup.h>
#include<sa/tcfile.h>
#include<ae/dataset.h>
#include<ae/datasettype.h>
#include<bom/bom.h>
#include "Header.h"

#define LOGIN ITK_init_module("izn","izn","dba")

int ITK_user_main(int argc, char** argv)
{
	int iFail = 0;
	int nfound = 0;
	tag_t tag_dataset = NULLTAG;
	tag_t tag_new_file = NULLTAG;
	tag_t *tag_ref_object = NULLTAG;


	iFail = LOGIN;
	if (iFail == 0)
	{
		printf("TC Login successful!\n---------------------\n\n");
		CHECK(AE_find_dataset2("hey", &tag_dataset));
		CHECK(AE_ask_dataset_named_refs(tag_dataset, &nfound, &tag_ref_object));
		CHECK(IMF_replace_file_and_get_new_tag(tag_ref_object[0], "C:\\Users\\T50361\\Desktop\\Assignments\\ITKstart.txt", TRUE, &tag_new_file));
		AOM_lock(tag_dataset);
		CHECK(AE_replace_dataset_named_ref2(tag_dataset, tag_ref_object[0], "Text", AE_PART_OF, tag_new_file));
		AE_save_myself(tag_dataset);
		AOM_unlock(tag_dataset);
		printf("Dataset named reference replaced successfully\n");
	}
	else
	{
		printf("TC Login Failed!\n-------------------------\n\n");
	}
	return ITK_ok;
}
















