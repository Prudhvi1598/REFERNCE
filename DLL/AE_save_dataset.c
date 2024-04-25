/* pre action to check whether the child line and the parent are of same type or not */

#include<stdio.h>
#include<stdlib.h>
#include<user_exits/user_exits.h>
#include<tccore/custom.h>
#include<ae/dataset_msg.h>
#include<ae/dataset.h>
#include<tccore/item_msg.h>
#include<tccore/item.h>
#include<tccore/aom_prop.h>


#define DLLAPI _declspec(dllexport)
#define EMH_error (EMH_USER_error_base +12)

int iFail = 0;
METHOD_id_t method_id;


//Declaration of function
extern DLLAPI int DLL_register_callbacks();
extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv);
extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv);
extern DLLAPI int add_post_action(METHOD_message_t *msg, va_list argv);

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
	iFail = METHOD_find_method("Dataset", AE_save_dataset_msg, &method_id);
	iFail = METHOD_add_action(method_id, METHOD_post_action_type, (METHOD_function_t)add_post_action, NULL);
	return iFail;
}

extern DLLAPI int add_post_action(METHOD_message_t *msg, va_list argv)
{
	tag_t dataset = va_arg(argv, tag_t);
	logical isnew = va_arg(argv, logical);

	int n_found = 0;
	tag_t *ref_object = NULL;
	/* New Dataset created */
	if (isnew == true || isnew==false)
	{
		AE_ask_dataset_named_refs(dataset, &n_found, &ref_object);
		if (n_found == 0)
		{
			EMH_store_error_s1(EMH_severity_error, EMH_error, NULL);
			return EMH_error;
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

