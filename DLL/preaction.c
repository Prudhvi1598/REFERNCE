/* Template to regester a DLL */
/* While deleting ItemRevision add its secondary objects to the previous revision */

#include<user_exits/user_exits.h>
#include<tc/tc_errors.h>
#include<tccore/method.h>
#include<tccore/item.h>
#include<tccore/custom.h>
#include<tccore/aom.h>
#include<tccore/aom_prop.h>
#include<stdio.h>
#include<tccore/grm.h>
#include<Header.h>

#define DLLAPI _declspec(dllexport)
#define PLM_error (EMH_USER_error_base +2)

METHOD_id_t tmethod_id;
int iFail = 0;


//Declaration of function
extern DLLAPI int DLL_register_callbacks();
extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv);
extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv);
extern DLLAPI int PLM_add_pre_action(METHOD_message_t *msg, va_list args);

//Definition of function
extern DLLAPI int DLL_register_callbacks()
{
	iFail = CUSTOM_register_exit("DLL", "USER_init_module", (CUSTOM_EXIT_ftn_t)PLM_execute_callbacks1);
	iFail = CUSTOM_register_exit("DLL", "USER_exit_module", (CUSTOM_EXIT_ftn_t)PLM_execute_callbacks2);
	return iFail;
}

extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n\n*****Welcome to DLL Registration*****\n\n\n");
	printf("Login success\n");
	iFail = METHOD_find_method("ItemRevision", "IMAN_delete", &tmethod_id);
	iFail = METHOD_add_action(tmethod_id, METHOD_pre_action_type, (METHOD_function_t)PLM_add_pre_action, NULL);
	return iFail;
}
//pre action
extern DLLAPI int PLM_add_pre_action(METHOD_message_t *msg, va_list args)
{
	tag_t source_rev = NULLTAG;
	tag_t tag_item= NULLTAG;
	tag_t *rev_list= NULLTAG;
	tag_t rel_type = NULLTAG;
	tag_t *secondary_obj = NULLTAG;
	tag_t relation = NULLTAG;
	int n_count = 0;
	int n_items = 0;
	char *object_type = NULL;

	source_rev = va_arg(args, tag_t);
	ITEM_ask_item_of_rev(source_rev, &tag_item);
	ITEM_list_all_revs(tag_item, &n_items, &rev_list);
	for (int i = 0; i < n_items; i++)
	{
		if (source_rev == rev_list[i])
		{
			GRM_find_relation_type("IMAN_specification", &rel_type);
			GRM_list_secondary_objects_only(source_rev, NULLTAG, &n_count, &secondary_obj);
			for (int j = 0; j < n_count; j++)
			{
				WSOM_ask_object_type2(secondary_obj[j], &object_type);
				if (strcmp(object_type, "ItemRevision Master") != 0)
				{
					GRM_create_relation(rev_list[i - 1], secondary_obj[j], rel_type, NULLTAG, &relation);
					GRM_save_relation(relation);
				}
			}
		}
	}
	
	return iFail;
}

extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n\n*****DLL logout success*****\n\n\n");
	return iFail;
}

