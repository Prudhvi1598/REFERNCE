#include<stdio.h>
#include<tcinit/tcinit.h>
#include<tccore/item.h>
#include<tccore/aom.h>
#include<tc/tc_startup.h>

#define user "infodba"
#define pwd "infodba"
#define grp "dba"

int ITK_user_main(int argc, char** argv)
{
	int iFail = NULL;
	tag_t trevision = NULLTAG;
	iFail = ITK_init_module(user, pwd, grp);
	if (iFail == ITK_ok)
	{
		printf("TC Login Successful!");
	}
	else
	{
		printf("TC Login Unsuccessful");
	}
	ITEM_find_rev("000000", "A",&trevision);
	if (trevision != NULLTAG)
	{
		AOM_refresh(trevision, true);
		ITEM_set_rev_description(trevision, "hello123");
		AOM_save_without_extensions(trevision);
		AOM_refresh(trevision, false);
		printf("ItemRevision Description changed succesfully");
	}
	else
	{
		printf("ItemRevision NotFound");
	}
	return ITK_ok;
}