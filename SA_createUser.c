#include<stdio.h>
#include<tcinit/tcinit.h>
#include<tc/tc_startup.h>
#include<bom/bom.h>
#include<sa/sa.h>
#include<sa/group.h>
#include "Header.h"

#define LOGIN ITK_init_module("izn","izn","dba")
#define LOGIN ITK_init_module("izn","izn","dba")

int ITK_user_main(int argc, char** argv)
{
	int iFail = 0;
	tag_t tag_user = NULLTAG;
	tag_t tag_group = NULLTAG;
	tag_t tag_person = NULLTAG;


	iFail = LOGIN;
	if (iFail == 0)
	{
		printf("TC Login successful!\n---------------------\n\n");
		CHECK(SA_create_person2("test_user1", &tag_person));
		AOM_save_without_extensions(tag_person);
		printf("person created\n");

		CHECK(SA_find_group("Cinema", &tag_group));
		CHECK(SA_create_licensed_user3("test_user1", "test_user1", "test_user1", 1, NULL, 1, &tag_user));
		CHECK(SA_set_user_login_group(tag_user, tag_group));
		CHECK(SA_set_os_user_name2(tag_user, "Windows"));
		AOM_save_without_extensions(tag_user);
		printf("user created\n");
	}
	else
	{
		printf("TC Login Failed!\n-------------------------\n\n");
	}
	return ITK_ok;
}