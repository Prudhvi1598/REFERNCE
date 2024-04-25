#include<stdio.h>
#include<tcinit/tcinit.h>
#include<tc/tc_startup.h>
#include<tccore/item.h>
#include<tccore/aom.h>
#include<bom/bom.h>

#define LOGIN ITK_init_module("izn","izn","dba")
#define BOM_option_mode_enum   PS_variant_option_mode_dynamic_comparison


int ITK_user_main(int argc, char** argv)
{
	int iFail = 0;
	tag_t titem = NULLTAG;
	iFail = LOGIN;
	if (iFail == 0)
	{
		printf("TC Login successful!\n----------------\n\n");
		ITEM_find_item("000346", &titem);
		BOM_create_option(titem,"ITK2",NULL,0,)
	}
	else
	{
		printf("TC Login Failed!\n----------------\n\n");
	}
}