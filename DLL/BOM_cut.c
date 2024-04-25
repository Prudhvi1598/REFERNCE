/* Template to regester a DLL */
#include<stdio.h>
#include<user_exits/user_exits.h>
#include<tc/tc_errors.h>
#include<tccore/method.h>
#include<bom/bom_msg.h>
#include<tccore/custom.h>
#include <tccore/aom_prop.h>


#define DLLAPI _declspec(dllexport)
#define EMH_error (EMH_USER_error_base +1)
int iFail = 0;

//Declaration of function
extern DLLAPI int DLL_register_callbacks();
extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv);
extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv);
extern DLLAPI int eventBOMLineCut(METHOD_message_t* msg, va_list argv);


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
	METHOD_id_t method;
	METHOD_find_method("BOMLine", BOMLine_cut_msg, &method);
	if (method.id != 0)
	{
		METHOD_add_action(method, METHOD_pre_action_type, (METHOD_function_t)eventBOMLineCut, NULL);
	}
	return iFail;
}

extern DLLAPI int eventBOMLineCut(METHOD_message_t* msg, va_list argv)
{
	va_list largs;
	va_copy(largs, argv);
	tag_t line_tag = va_arg(largs, tag_t);
	va_end(largs);

	char* value = NULL;

	AOM_ask_value_string(line_tag, "bl_substitute_list", &value);
	if (tc_strcmp(value, "") != 0);
	else
	{
		EMH_store_error(EMH_severity_error, EMH_error);
		return EMH_error;
	}

}

extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n\n*****DLL logout success*****\n\n\n");
	return iFail;
}
