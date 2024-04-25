#include<stdio.h>
#include<tcinit/tcinit.h>
#include<tc/tc_startup.h>
#include<bom/bom.h>
#include<cfm/cfm.h>
#include<tccore/aom.h>

#define LOGIN ITK_init_module("izn","izn","dba")

int ITK_user_main(int argc, char** argv)
{
	int iFail = 0;
	tag_t tnewrule = NULLTAG;
	tag_t tenditem = NULLTAG;
	tag_t tentry = NULLTAG;

	iFail = LOGIN;
	if (iFail == 0)
	{
		printf("TC Login successful!\n---------------------\n\n");
		CFM_rule_create("ITKrule", NULL, &tnewrule);
		AOM_save_without_extensions(tnewrule);
		if (tnewrule != NULLTAG)
		{
			printf("Revision rule created succesfully!n\n");
			CFM_rule_add_entry(tnewrule, 8, &tentry);
			ITEM_find_item("000357", &tenditem);
			CFM_rule_set_end_item(tnewrule, tenditem);
			tentry = NULLTAG;
			CFM_rule_add_entry(tnewrule, 4, &tentry);
			CFM_rule_set_unit(tnewrule, 5);
			tentry = NULLTAG;
			CFM_rule_add_entry(tnewrule, 0, &tentry);
			AOM_save_without_extensions(tnewrule);
		}
	}
	else
	{
		printf("TC Login Failed!\n-------------------------\n\n");
	}
	return ITK_ok;
}