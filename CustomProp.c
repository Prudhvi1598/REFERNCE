#include<stdio.h>
#include<tccore/aom.h>
#include<tc/tc_startup.h>
#include<tcinit/tcinit.h>
#include<tccore/item.h>
#include<tccore/aom_prop.h>

#define user "infodba"
#define pwd "infodba"
#define grp "dba"

int ITK_user_main(int argc, char** argv)
{
	int iFail = 0;
	tag_t titem = NULLTAG;
	iFail = ITK_init_module(user, pwd, grp);
	if (iFail == ITK_ok)
	{
		printf("TC Login Successful!");
		ITEM_find_item("000326", &titem);
		if (titem != 0)
		{
			AOM_refresh(titem, true);
			AOM_set_value_string(titem, "ks2location","HYDERABAD");
			ITEM_save_item(titem);
			AOM_refresh(titem, false);
			printf("\n\nProperty set successfully!");
		}
		else
		{
			printf("\n\nItem Not Found\n\n");
		}
	}
	else
	{
		printf("TC Login Unsuccessful");
	}

	return ITK_ok;
}