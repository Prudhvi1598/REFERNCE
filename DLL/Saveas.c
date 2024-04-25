/* Template to regester a DLL */
/* After performing save as operation on ItemRevision, its form attribute should be updated */
#include<user_exits/user_exits.h>
#include<tc/tc_errors.h>
#include<tccore/method.h>
#include<tccore/custom.h>
#include<tccore/aom.h>
#include<tccore/aom_prop.h>
#include<stdio.h>
#include<tccore/grm.h>

#define DLLAPI _declspec(dllexport)
#define PLM_error (EMH_USER_error_base +2)

METHOD_id_t tmethod_id;
int iFail = 0;


//Declaration of function
extern DLLAPI int DLL_register_callbacks();
extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv);
extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv);
extern DLLAPI int PLM_add_post_action(METHOD_message_t *msg, va_list args);

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
	iFail = METHOD_find_method("ItemRevision", "ITEM_create_from_rev", &tmethod_id);
	iFail = METHOD_add_action(tmethod_id, METHOD_post_action_type, (METHOD_function_t)PLM_add_post_action, NULL);
	return iFail;
}
//post action
extern DLLAPI int PLM_add_post_action(METHOD_message_t *msg, va_list args)
{
	tag_t source_rev = NULLTAG;
	tag_t rel_type = NULLTAG;
	tag_t *secondary_obj = NULLTAG;
	int n_count = 0;
	char *pvalue = NULL;
	char *pname = NULL;
	source_rev = va_arg(args, tag_t);
	GRM_find_relation_type("IMAN_master_form_rev", &rel_type);
	GRM_list_secondary_objects_only(source_rev, rel_type, &n_count, &secondary_obj);
	if (n_count != 0)
	{
		for (int i = 0; i < n_count; i++)
		{
			AOM_refresh(secondary_obj[i], true);
			AOM_set_value_string(secondary_obj[i], "user_data_1", "DLL");
			AOM_save_without_extensions(secondary_obj[i]);
			AOM_refresh(secondary_obj[0], false);
		}
	}
	else
	{
		EMH_store_error_s1(EMH_severity_error, PLM_error, pname);
		return PLM_error;
	}
	return iFail;
}

extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n\n*****DLL logout success*****\n\n\n");
	return iFail;
}

