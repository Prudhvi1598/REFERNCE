/* Template to regester a DLL */
/* while performing save as operation on Item, its Master form property length should not > 5 in length  */

#include<user_exits/user_exits.h>
#include<tc/tc_errors.h>
#include<tccore/method.h>
#include<tccore/item.h>
#include<tccore/custom.h>
#include<tccore/aom.h>
#include<ae/dataset.h>
#include<ae/datasettype.h>
#include<tccore/aom_prop.h>
#include<fclasses/tc_string.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<tccore/grm.h>
#include<Header.h>

#define DLLAPI _declspec(dllexport)
#define PLM_error (EMH_USER_error_base +3)

METHOD_id_t tmethod_id;
int iFail = ITK_ok;


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
	iFail = METHOD_find_method("Item", "ITEM_create_from_rev", &tmethod_id);
	iFail = METHOD_add_action(tmethod_id, METHOD_pre_action_type, (METHOD_function_t)PLM_add_pre_action, NULL);
	return iFail;
}
//pre action
extern DLLAPI int PLM_add_pre_action(METHOD_message_t *msg, va_list args)
{
	tag_t old_item= NULLTAG;
	old_item = va_arg(args, tag_t);

	tag_t old_rev= NULLTAG;
	old_rev = va_arg(args, tag_t);

	char *new_item_id = NULL;
	new_item_id = va_arg(args, char *);
	
	char *new_rev_id = NULL;
	new_rev_id = va_arg(args, char *);

	tag_t new_item = NULLTAG;
	new_item = va_arg(args, tag_t);

	tag_t rel_type = NULLTAG;
	tag_t *secondary_obj = NULLTAG;
	int n_count = 0;
	char *value = NULL;//OF
	

	GRM_find_relation_type("IMAN_master_form_rev", &rel_type);
	GRM_list_secondary_objects_only(old_item, rel_type, &n_count, &secondary_obj);
	AOM_ask_value_string(secondary_obj[0], "item_comment", &value);
	if(tc_strlen(value)>=5)
		AOM_save_without_extensions(new_item);
	else
	{
		EMH_store_error_s1(EMH_severity_error, PLM_error, new_item_id);
		return PLM_error;
	}
}

extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n\n*****DLL logout success*****\n\n\n");
	return iFail;
}

