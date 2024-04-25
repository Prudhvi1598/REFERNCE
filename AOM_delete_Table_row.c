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
	tag_t tag_item = NULLTAG;
	tag_t *tag_table_row = NULLTAG;
	tag_t *tag_table_row1 = NULLTAG;
	char *char_id = NULL;


	iFail = LOGIN;
	if (iFail == 0)
	{
		printf("TC Login successful!\n---------------------\n\n");
		CHECK(ITEM_find_item("000366", &tag_item));
		/*CHECK(AOM_ask_table_rows(tag_item, "ks2_TimeSheet", &n_rows, &tag_table_row));
		printf("The no of rows in a table is %d\n", n_rows);
		for (int i = 0; i < n_rows; i++)
		{
			WSOM_ask_id_string(tag_table_row[i], &char_id);
			printf("%s\n", char_id);
			char *char_id = NULL;
		}
		//AOM_insert_table_rows(tag_item, "ks2_TimeSheet", 2, 1, &tag_table_row1); */
		AOM_refresh(tag_item, true);
		CHECK(AOM_delete_table_rows(tag_item, "ks2_TimeSheet", 1, 1));
		AOM_refresh(tag_item, false);
	}
	else
	{
		printf("TC Login Failed!\n-------------------------\n\n");
	}
	return ITK_ok;
}
