#include<stdio.h>
#include<tcinit/tcinit.h>
#include<tc/tc_startup.h>
#include<tccore/aom.h>
#include<tccore/aom_prop.h>
#include<bom/bom.h>
#include "Header.h"

#define LOGIN ITK_init_module("izn","izn","dba")

int ITK_user_main(int argc, char** argv)
{
	int iFail = 0;
	int n_rows = 0;
	int index = 0;
	tag_t tag_item = NULLTAG;
	tag_t *tag_table_rows = NULL;
	char *name = NULL;
	int Employee_id[3] = { 111,222,333 };
	char *Employee_names[] = { "suresh","hrithik","akhil" };
	date_t dates[] = { "11-Aug-2025 15:30","12-Aug-2025 15:30" ,"13-Aug-2025 15:30" };
	char *locations[] = { "hyd","chennai","Mumbai" };

	iFail = LOGIN;
	if (iFail == 0)
	{
		printf("TC Login successful!\n---------------------\n\n");
		CHECK(ITEM_find_item("000366", &tag_item));
		AOM_refresh(tag_item, true);
		CHECK(AOM_insert_table_rows(tag_item, "ks2_TimeSheet", 0, 3, &tag_table_rows));
		for (int i = 0; i < 3; i++)
		{
			CHECK(AOM_set_value_int(tag_table_rows[i], "ks2EMP_ID", Employee_id[i]));
			CHECK(AOM_set_value_string(tag_table_rows[i], "ks2EMP_Name",Employee_names[i]));
			CHECK(AOM_set_value_date(tag_table_rows[i], "ks2Date", dates[i]));
			CHECK(AOM_set_value_string(tag_table_rows[i], "ks2Office_Location", locations[i]));
			AOM_save_without_extensions(tag_table_rows[i]);
			AOM_save_without_extensions(tag_item);
			printf("Row %d set succesful\n", i + 1);
		}
		AOM_refresh(tag_item, false);
	}
	else
	{
		printf("TC Login Failed!\n-------------------------\n\n");
	}
	return ITK_ok;
}
