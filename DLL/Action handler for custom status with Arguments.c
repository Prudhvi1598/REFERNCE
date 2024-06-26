/* Template to regester a DLL */
/* To create Action handler for custom Release status with arguments */

#include
#include<Header.h>
#include<stdio.h>
#include<string.h>
#include<bom/bom.h>
#include<stdlib.h>
#include<user_exits/user_exits.h>
#include<tc/tc_errors.h>
#include<tc/tc_arguments.h>
#include<tc/tc_startup.h>
#include<tccore/method.h>
#include<tccore/item.h>
#include<tccore/aom.h>
#include<tccore/custom.h>
#include<pom/pom/pom.h>
#include<tccore/aom_prop.h>
#include<epm/epm.h>
#include<fclasses/tc_string.h>
#include<tccore/releasestatus.h>



#define DLLAPI _declspec(dllexport)
#define PLM_error (EMH_USER_error_base +4)

METHOD_id_t tmethod_id;
int iFail = ITK_ok;


//Declaration of function
extern DLLAPI int DLL_register_callbacks();
extern DLLAPI int Handler_execute_callbacks1(int *decision, va_list argv);
extern DLLAPI int Handler_execute_callbacks2(int *decision, va_list argv);
extern DLLAPI int add_action_handler(EPM_action_message_t msg);

//Definition of function
extern DLLAPI int DLL_register_callbacks()
{
	iFail = CUSTOM_register_exit("DLL", "USER_init_module", (CUSTOM_EXIT_ftn_t)Handler_execute_callbacks1);
	iFail = CUSTOM_register_exit("DLL", "USER_exit_module", (CUSTOM_EXIT_ftn_t)Handler_execute_callbacks2);
	return iFail;
}

extern DLLAPI int Handler_execute_callbacks1(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n\n*****Welcome to Action Handler DLL Registration*****\n\n\n");
	printf("Login success\n");
	EPM_register_action_handler("Custom-status-Release-with-Arguments", "Custom Action Handler with arguments", (EPM_action_handler_t)add_action_handler);
	return iFail;
}
//Action Handler with arguments
extern DLLAPI int add_action_handler(EPM_action_message_t msg)
{
	int count = 0;
	int no_of_arguments = 0;
	tag_t root_task = NULLTAG;
	tag_t release_status = NULLTAG;
	tag_t *attachments = NULL;
	tag_t class_id = NULLTAG;
	char *class_name = NULL;
	char *argument_name = NULL;
	char *argument_value = NULL;
	char *status = NULL;

	CHECK(EPM_ask_root_task(msg.task, &root_task));
	CHECK(EPM_ask_attachments(root_task, EPM_target_attachment, &count, &attachments));
	no_of_arguments = TC_number_of_arguments(msg.arguments);
	for (int i = 0; i < no_of_arguments; i++)
	{
		ITK_ask_argument_named_value(TC_next_argument(msg.arguments), &argument_name, &argument_value);
		if (tc_strcmp(argument_name, "apply_status") == 0)
		{
			status = (char *)MEM_alloc(100);
			tc_strcpy(status, argument_value);
		}
	}
	for (int i = 0; i < count; i++)
	{
		POM_class_of_instance(attachments[i], &class_id);
		POM_name_of_class(class_id, &class_name);
		if (tc_strcmp(class_name, "ItemRevision") == 0)
		{
			CHECK(RELSTAT_create_release_status(status, &release_status));
			CHECK(RELSTAT_add_release_status(release_status, 1, attachments, true));
		}
		else
		{
			EMH_store_error_s1(EMH_severity_error, PLM_error, class_name);
			return PLM_error;
		}
	}
	return ITK_ok;
}

extern DLLAPI int Handler_execute_callbacks2(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf("\n\n\n*****DLL logout success*****\n\n\n");
	return iFail;
}


