/* Incomplete */
/* To check out Dataset, make sure it has Named ref. */

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
#include<ae/ae.h>
#include<tccore/grm.h>

#define DLLAPI _declspec(dllexport)
#define EMH_error (EMH_USER_error_base +12)

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
	iFail = METHOD_find_method("ItemRevision", "IMAN_refresh", &tmethod_id);
	iFail = METHOD_add_action(tmethod_id, METHOD_pre_action_type, (METHOD_function_t)PLM_add_pre_action, NULL);
	return iFail;
}
//pre action
extern DLLAPI int PLM_add_pre_action(METHOD_message_t *msg, va_list args)
{
	tag_t object = va_arg(args, tag_t);
	logical lock= va_arg(args, logical);
	int n_found = 0;
	int count = 0;
	tag_t class_id = NULLTAG;
	tag_t *ref_object = NULL;
	tag_t *secondary_objects = NULL;
	char *value = NULL;
	char *class_name = NULL;

	GRM_list_secondary_objects_only(object, NULLTAG, &count, &secondary_objects);
	for (int i = 0; i < count; i++)
	{
		POM_class_of_instance(secondary_objects[i], &class_id);
		POM_name_of_class(class_id, &class_name);
		if (tc_strcmp(class_name, "Dataset") == 0)
		{
			AE_ask_dataset_named_refs(secondary_objects[i], &n_found, &ref_object);
			if (n_found == 0)
			{
				EMH_store_error_s1(EMH_severity_error, EMH_error, value);
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

