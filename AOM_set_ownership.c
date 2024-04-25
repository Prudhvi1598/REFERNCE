#include<stdio.h>
#include<tcinit/tcinit.h>
#include<tc/tc_startup.h>
#include<tccore/aom.h>
#include<sa/user.h>
#include<sa/group.h>
#include<bom/bom.h>
#include "Header.h"

#define LOGIN ITK_init_module("izn","izn","dba")

int ITK_user_main(int argc, char** argv)
{
	int iFail = 0;
	tag_t tag_rev = NULLTAG;
	tag_t tag_user = NULLTAG;
	tag_t tag_group = NULLTAG;

	iFail = LOGIN;
	if (iFail == 0)
	{
		printf("TC Login successful!\n---------------------\n\n");
		CHECK(ITEM_find_rev("000321", "C", &tag_rev));
		CHECK(SA_find_user2("infodba", &tag_user));
		CHECK(SA_find_group("dba",&tag_group));
		ITK_set_bypass(true);  /* To set ownership for released objects, we need to use this API */
		CHECK(AOM_set_ownership(tag_rev, tag_user, tag_group));
		ITK_set_bypass(false);
		printf("Ownership changed succesfully!");
	}
	else
	{
		printf("TC Login Failed!\n-------------------------\n\n");
	}
	return ITK_ok;
}














