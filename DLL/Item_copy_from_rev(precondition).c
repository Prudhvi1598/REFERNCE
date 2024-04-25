/* To Update Item master form rev property after save as operation */
#include <stdio.h>
#include <conio.h>
#include <tccore/custom.h>
#include <tccore/method.h>
#include<user_exits/user_exits.h>
#include<tccore/grm.h>
#include<tccore/aom_prop.h>
#include<tccore/aom.h>

#define DLLAPI _declspec(dllexport)
int iFail = 0;
METHOD_id_t method_id;

//Declaration of function 
extern DLLAPI DLL_register_callbacks();
extern DLLAPI execute_callbacks1(int* decision, va_list agrs);
extern DLLAPI execute_callbacks2(int* decision, va_list agrs);
extern DLLAPI post_condition(METHOD_message_t* msg, va_list args);

extern DLLAPI DLL_register_callbacks()
{
	iFail = CUSTOM_register_exit("DLL", "USER_init_module", (CUSTOM_EXIT_ftn_t)execute_callbacks1);
	iFail = CUSTOM_register_exit("DLL", "USER_exit_module", (CUSTOM_EXIT_ftn_t)execute_callbacks2);
	return iFail;
}

extern DLLAPI execute_callbacks1(int* decision, va_list agrs)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n\n******** Welcome To Register Process ******* ");
	printf("\n\n\n ******** Login Sucessfull ********");
	iFail = METHOD_find_method("Item", "ITEM_create_from_rev", &method_id);
	iFail = METHOD_add_action(method_id, METHOD_post_action_type, (METHOD_function_t)post_condition, NULL);
	return iFail;
}

extern DLLAPI post_condition(METHOD_message_t* msg, va_list args)
{
	tag_t old_item = va_arg(args, tag_t);
	tag_t old_rev = va_arg(args, tag_t);
	const char* new_item_id = va_arg(args, const char*);
	const char* new_rev_id = va_arg(args, const char*);
	tag_t* new_item = va_arg(args, tag_t*);
	tag_t* new_rev = va_arg(args, tag_t*);
	const char* new_name = va_arg(args, const char*);
	const char*new_description = va_arg(args, const char*);

	tag_t relation_type = NULLTAG;
	int count = 0;
	tag_t *secondary_objects = NULL;

	GRM_find_relation_type("IMAN_master_form_rev", &relation_type);
	GRM_list_secondary_objects_only(*new_rev, relation_type, &count, &secondary_objects);
	AOM_refresh(secondary_objects[0], true);
	AOM_set_value_string(secondary_objects[0], "item_comment", "PROPERTY UPDATED!!!");
	AOM_save_without_extensions(secondary_objects[0]);
	AOM_refresh(secondary_objects[0],false);

	return iFail;

}

extern DLLAPI execute_callbacks2(int* decision, va_list agrs)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n\n ******** Logout Success ********");
	return iFail;
}