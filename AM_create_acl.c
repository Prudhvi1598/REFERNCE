#include<stdio.h>
#include<tcinit/tcinit.h>
#include<tc/tc_startup.h>
#include<bom/bom.h>
#include<sa/am.h>
#include "Header.h"

#define LOGIN ITK_init_module("izn","izn","dba")

int ITK_user_main(int argc, char** argv)
{
	int iFail = 0;
	int n_types = 0;
	tag_t tag_acl_id = NULLTAG;
	tag_t tag_acc = NULLTAG;
	tag_t tag_write = NULLTAG;
	char **char_types = NULL;

	iFail = LOGIN;
	if (iFail == 0)
	{
		printf("TC Login successful!\n---------------------\n\n");
		CHECK(AM_create_acl("kumar", &tag_acl_id));
		CHECK(AM_find_accessor("World", "World", &tag_acc));
		CHECK(AM_find_privilege("Write", &tag_write));
		CHECK(AM_grant_privilege(tag_acl_id, tag_acc, tag_write));
		AM_save_acl(tag_acl_id);
		printf("ACL created in tree\n");
		/*CHECK(AM_list_accessor_types(&n_types, &char_types));
		for (int i = 0; i < n_types; i++)
		{
			printf("%s\n", char_types[i]);
		}
		*/
	}
	else
	{
		printf("TC Login Failed!\n-------------------------\n\n");
	}
	return ITK_ok;
}
















