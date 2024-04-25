#include<stdio.h>
#include<tcinit/tcinit.h>
#include<tc/tc_startup.h>
#include<tccore/tctype.h>
#include<tccore/item.h>
#include<bom/bom.h>
#include<tc/folder.h>

#define LOGIN ITK_init_module("izn","izn","dba")

int ITK_user_main(int argc, char** argv)
{
	int iFail = 0;
	iFail = LOGIN;
	tag_t object_type_tag = NULLTAG;
	tag_t create_input_tag = NULLTAG;
	tag_t bo = NULLTAG;
	const char *object_name[] = { "TestItemName" };


	if (iFail == 0)
	{
		printf("TC Login successful!\n---------------------\n\n");
		TCTYPE_find_type("Item", NULL, &object_type_tag);
		if (object_type_tag != NULLTAG)
		{
			printf("Object type tag created\n\n");
			TCTYPE_construct_create_input(object_type_tag, &create_input_tag);
			if (create_input_tag != NULLTAG)
			{
				printf("Create input type tag created\n\n");
				TCTYPE_set_create_display_value(create_input_tag, "object_name", 1, object_name);
				TCTYPE_create_object(create_input_tag, &bo);
				ITEM_save_item(bo);
				if (bo != NULLTAG)
				{
					FL_user_update_newstuff_folder(bo);
					printf("Business object created!\n\n");
				}
					
				else
					printf("Business object not created!\n\n");
			}
			else
			{
				printf("Create input type tag not created\n\n");
			}
		}
		else
		{
			printf("Object type tag not created\n\n");
		}
	}
	else
	{
		printf("TC Login Failed!\n-------------------------\n\n");
	}
	return ITK_ok;
}