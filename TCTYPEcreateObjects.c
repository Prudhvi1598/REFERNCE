#include<stdio.h>
#include<tcinit/tcinit.h>
#include<tc/tc_startup.h>
#include<tccore/tctype.h>
#include<tccore/aom.h>
#include<tccore/item.h>
#include<bom/bom.h>
#include<tc/folder.h>

#define LOGIN ITK_init_module("izn","izn","dba")

int ITK_user_main(int argc, char** argv)
{
	int iFail = 0;
	int n_objects;
	int quantity[] = { 3,2 };
	tag_t object_type_tag = NULLTAG;
	tag_t object_type_tag1 = NULLTAG;
	tag_t create_input_tag = NULLTAG;
	tag_t create_input_tag1 = NULLTAG;
	tag_t *objects_t=NULL;
	

	iFail = LOGIN;
	if (iFail == 0)
	{
		printf("TC Login successful!\n---------------------\n\n");
		TCTYPE_find_type("Item", NULL, &object_type_tag);
		TCTYPE_find_type("Form", NULL, &object_type_tag1);
		if (object_type_tag != NULLTAG)
		{
			printf("Object type tag created\n\n");
			TCTYPE_construct_create_input(object_type_tag, &create_input_tag);
			TCTYPE_construct_create_input(object_type_tag1, &create_input_tag1);
			if (create_input_tag != NULLTAG)
			{
				printf("Create input type tag created\n\n");
				tag_t create_inputs[] = { create_input_tag,create_input_tag1 };
				TCTYPE_create_objects(2, quantity,create_inputs , &n_objects, &objects_t);
				if (objects_t != NULL)
				{
					for (int i = 0; i < n_objects; i++)
					{
						ITEM_save_item(objects_t[i]);
						FL_user_update_newstuff_folder(objects_t[i]);
						printf("Business object created!\n\n");
					}
				}
				else
					printf("Business objects not created!\n\n");
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