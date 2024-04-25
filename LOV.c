#include<stdio.h>
#include<string.h>
#include<tcinit/tcinit.h>
#include<tc/tc_startup.h>
#include<tccore/aom_prop.h>
#include<lov/lov.h>
#include<bom/bom.h>
#include "Header.h"

#define LOGIN ITK_init_module("izn","izn","dba")

int ITK_user_main(int argc, char** argv)
{
	int iFail = 0;
	int n_values = 0;
	char **char_values=NULL;
	tag_t tag_item = NULLTAG;
	tag_t tag_lov = NULLTAG;


	iFail = LOGIN;
	if (iFail == 0)
	{
		
		printf("TC Login successful!\n---------------------\n\n");
		CHECK(ITEM_find_item("A001", &tag_item));
		AOM_refresh(tag_item, true);
		CHECK(AOM_ask_lov(tag_item, "ks2OfficeLocation", &tag_lov));
		CHECK(LOV_ask_values_string(tag_lov, &n_values, &char_values));
		for (int i = 0; i < n_values; i++)
		{
			printf("The values are: %s\n", char_values[i]);
			if (strcmp(char_values[i], "Bangalore") == 0)
			{
				AOM_set_value_string(tag_item, "ks2OfficeLocation", char_values[i]);
				AOM_save_without_extensions(tag_item);
				AOM_refresh(tag_item, false);
				printf("LOV value changed succesfully!\n\n");
			}
			else
				MEM_free(char_values[i]);
		}
	}
	else
	{
		printf("TC Login Failed!\n-------------------------\n\n");
	}
	return ITK_ok;
}
