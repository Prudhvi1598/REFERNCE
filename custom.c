#include<stdio.h>
#include<tcinit/tcinit.h>
#include<tc/tc_startup.h>
#include<tccore/aom.h>
#include<sa/user.h>
#include<tc/folder.h>
#include<bom/bom.h>


#define LOGIN ITK_init_module("izn","izn","dba")

int ITK_user_main(int argc, char** argv)
{
	int iFail = 0;
	tag_t tag_item = NULLTAG;
	tag_t tag_rev = NULLTAG;
	tag_t tag_user = NULLTAG;
	tag_t tag_home = NULLTAG;
	char *attributes_names[] = { "object_desc","object_name" };
	char *attribute_values[] = { "ITK","Hrithik" };


	iFail = LOGIN;
	if (iFail == 0)
	{
		printf("TC Login successful!\n---------------------\n\n");
		CHECK(ITEM_create_item2("A2CustomItem", 1, attributes_names, attribute_values, NULL, &tag_item, &tag_rev));
		CHECK(SA_find_user2("izn", &tag_user));
		CHECK(SA_ask_user_home_folder(tag_user, &tag_home));
		CHECK(AOM_save_without_extensions(tag_item));
		CHECK(FL_insert(tag_home, tag_item, 999));
		CHECK(AOM_save_without_extensions(tag_home));
		printf("Custom object created!\n");
	}
	else
	{
		printf("TC Login Failed!\n-------------------------\n\n");
	}
	return ITK_ok;
}
















