#include<stdio.h>
#include<tccore/method.h>
#include<tccore/custom.h>
#include<user_exits/user_exits.h>
#include<tc/tc_errors.h>

#define DLLAPI _declspec(dllexport)
#define PLM_error (EMH_USER_error_base +2)

METHOD_id_t tmethod_id;
int iFail = 0;

extern DLLAPI int DLL_register_callbacks();
extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv);
extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv);

extern DLLAPI int DLL_register_callbacks()
{
	iFail = CUSTOM_register_exit("DLL", "USER_init_module", (CUSTOM_EXIT_ftn_t)PLM_execute_callbacks1);
	iFail = CUSTOM_register_exit("DLL", "USER_exit_module", (CUSTOM_EXIT_ftn_t)PLM_execute_callbacks2);
}

extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n\n*****Welcome to DLL Registration*****\n\n\n");
	printf("Login success\n");
	iFail = METHOD_find_method("A2CustomItem", "ITEM_copy_rev", &tmethod_id);
	iFail = METHOD_add_pre_condition(tmethod_id, (METHOD_function_t)PLM_add_precondition, NULL);
	return iFail;
}

extern DLLAPI int PLM_add_precondition(METHOD_message_t *msg, va_list args)
{
	tag_t source_rev = NULLTAG;
	char *pvalue = NULL;
	char *pname = NULL;
	source_rev = va_arg(args, tag_t);
	AOM_UIF_ask_value(source_rev, "release_status_list", &pvalue);
	AOM_UIF_ask_value(source_rev, "object_name", &pname);
	if (tc_strcmp(pvalue, "TCM Released") == 0)
		printf("TCM released\n\n");
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